#pragma once
#include "type.hpp"


namespace cherry::syn {

/// @brief   Capable of parsing a simple path.
/// @details A simple path is a single identifier followed by any number of
///          other identifiers preceded by the access operator. For example,
///          `std.io.file`.
/// @tparam  Lexer The type of the lexer that will provide the tokens for the
///          parser to syntactically analyze.
template<detail::LexicalAnalyzer Lexer>
struct simple_path_parser final {
    static result<ast::simple_path>
    parse(state<Lexer>& ctx) noexcept {
        if (ctx.current.type != leaf::identifier)
            return std::unexpected(errc::not_my_syntax);
        auto node = ast::simple_path{};
        node.segments.push_back(ctx.current.lexeme);

        // While there is an access operator, keep adding segments.
        ctx.next_token();
        while (ctx.current.type == leaf::op_access) {
            ctx.next_token();
            if (ctx.current.type != leaf::identifier)
                return std::unexpected(errc::expected_identifier);
            node.segments.push_back(ctx.current.lexeme);
            ctx.next_token();
        }
        return node;
    }
};


/// @brief   Capable of parsing a path segment.
/// @details A path segment is a syntax construct that acts as the intermediary
///          pieces of a path expression or type expression.
/// @tparam  Lexer The type of the lexer that will provide the tokens for the
///          parser to syntactically analyze.
template<detail::LexicalAnalyzer Lexer>
struct segment_parser final {
    static result<uptr_t<ast::segment>>
    parse(state<Lexer>& ctx) noexcept {
        if (identifier(ctx.current.type))
            return parse_generics(ctx);
        
        if (primitive(ctx.current.type))
            return parse_primitive(ctx);
        
        return std::unexpected(errc::not_my_syntax);
    }

private:
    static bool
    identifier(leaf type) noexcept {
        return type == leaf::identifier;
    }

    static bool
    primitive(leaf type) noexcept {
        switch (type) {
        case leaf::kw_bool:    [[fallthrough]];
        case leaf::kw_char:    [[fallthrough]];
        case leaf::kw_int8:    [[fallthrough]];
        case leaf::kw_int16:   [[fallthrough]];
        case leaf::kw_int32:   [[fallthrough]];
        case leaf::kw_int64:   [[fallthrough]];
        case leaf::kw_uint8:   [[fallthrough]];
        case leaf::kw_uint16:  [[fallthrough]];
        case leaf::kw_uint32:  [[fallthrough]];
        case leaf::kw_uint64:  [[fallthrough]];
        case leaf::kw_single:  [[fallthrough]];
        case leaf::kw_double:  [[fallthrough]];
        case leaf::kw_string:  [[fallthrough]];
        case leaf::kw_void:    return true;
        }

        return false;
    }

    static result<uptr_t<ast::segment>>
    parse_primitive(state<Lexer>& ctx) noexcept {
        auto segm = std::make_unique<ast::segment::primitive>();
        switch (ctx.current.type) {
        case leaf::kw_bool:   segm->val = ast::segment::primitive::p_bool;   break;
        case leaf::kw_char:   segm->val = ast::segment::primitive::p_char;   break;
        case leaf::kw_int8:   segm->val = ast::segment::primitive::p_int8;   break;
        case leaf::kw_int16:  segm->val = ast::segment::primitive::p_int16;  break;
        case leaf::kw_int32:  segm->val = ast::segment::primitive::p_int32;  break;
        case leaf::kw_int64:  segm->val = ast::segment::primitive::p_int64;  break;
        case leaf::kw_uint8:  segm->val = ast::segment::primitive::p_uint8;  break;
        case leaf::kw_uint16: segm->val = ast::segment::primitive::p_uint16; break;
        case leaf::kw_uint32: segm->val = ast::segment::primitive::p_uint32; break;
        case leaf::kw_uint64: segm->val = ast::segment::primitive::p_uint64; break;
        case leaf::kw_single: segm->val = ast::segment::primitive::p_single; break;
        case leaf::kw_double: segm->val = ast::segment::primitive::p_double; break;
        case leaf::kw_string: segm->val = ast::segment::primitive::p_string; break;
        case leaf::kw_void:   segm->val = ast::segment::primitive::p_void;   break;
        }

        ctx.next_token();
        return std::move(segm);
    }

    static result<uptr_t<ast::segment>>
    parse_generics(state<Lexer>& ctx) noexcept {
        auto segm = std::make_unique<ast::segment::generic>();
        segm->name = ctx.current.lexeme;
        ctx.next_token();

        // We expect to see '<>' or
        // <(Type,)* Type,?>
        if (ctx.current.type != leaf::op_logless)
            return segm;

        // Parse type arguments.
        do {
            ctx.next_token();

            // check type.tcc
            auto arg = type_parser<Lexer>::parse(ctx);
            if (!arg.has_value())
                // TODO: return appropriate error code.
                return std::unexpected(errc::failure);
            
            segm->inputs.emplace_back(std::move(arg.value()));
            if (ctx.current.type == leaf::dc_comma)
                continue;
        } while (ctx.current.type != leaf::op_logmore);

        return std::move(segm);
    }
};


/// @brief   Capable of parsing a path expression.
/// @details A path expression is a syntax construct that accesses modules,
///          submodules, object, or subobjects to reach a destinated path. It
///          can handle type generics along the way.
/// @tparam  Lexer The type of the lexer that will provide the tokens for the
///          parser to syntactically analyze.
template<detail::LexicalAnalyzer Lexer>
struct path_expr_parser final {
    static result<ast::path_expr>
    parse(state<Lexer>& ctx) noexcept {
        if (!identifier(ctx.current.type) && !primitive(ctx.current.type))
            return std::unexpected(errc::not_my_syntax);

        auto node = ast::path_expr{};
        auto segm = segment_parser<Lexer>::parse(ctx);
        if (!segm.has_value()) {
            auto error = segm.error();
            if (error == errc::not_my_syntax)
                return std::unexpected(error);

            // TODO: return appropriate error.
            return std::unexpected(errc::failure);
        }

        node.segments.emplace_back(std::move(segm.value()));
        while (ctx.current.type == leaf::op_access) {
            ctx.next_token();
            segm = segment_parser<Lexer>::parse(ctx);
            if (!segm.has_value())
                // TODO: return appropriate error.
                return std::unexpected(errc::failure);
            node.segments.emplace_back(std::move(segm.value()));
        }

        return node;
    }

private:
    static bool
    identifier(leaf type) noexcept {
        return type == leaf::identifier;
    }

    static bool
    primitive(leaf type) noexcept {
        switch (type) {
        case leaf::kw_bool:   [[fallthrough]];
        case leaf::kw_char:   [[fallthrough]];
        case leaf::kw_int8:   [[fallthrough]];
        case leaf::kw_int16:  [[fallthrough]];
        case leaf::kw_int32:  [[fallthrough]];
        case leaf::kw_int64:  [[fallthrough]];
        case leaf::kw_uint8:  [[fallthrough]];
        case leaf::kw_uint16: [[fallthrough]];
        case leaf::kw_uint32: [[fallthrough]];
        case leaf::kw_uint64: [[fallthrough]];
        case leaf::kw_single: [[fallthrough]];
        case leaf::kw_double: [[fallthrough]];
        case leaf::kw_string: [[fallthrough]];
        case leaf::kw_void:   return true;
        }

        return false;
    }
};

} // namespace cherry::syn


// Now define type parser function.
#include "type.tcc"
