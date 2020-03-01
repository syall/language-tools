// @ts-ignore
import { Token, Lexeme } from './utils.ts';

// Lexer
export class Lexer {

    // Utilities
    public createToken = (t: string, v: any, s: number, e: number): Token => ({
        type: t, value: v, start: s, end: e
    });
    public lexerError = (msg: string, start: number, end: number) => {
        console.error(`Error at ${start} to ${end}: ${msg}`);
        // @ts-ignore
        Deno.exit(1);
    };

    // Helpers
    public vary = (c: string) =>
        c >= 'a' && c <= 'z' ||
        c >= 'A' && c <= 'Z' ||
        c === '_' || c === '$';
    public digit = (c: string) => c >= '0' && c <= '9';
    public str = (c: string) => c === '"';
    public ws = (c: string) => c <= ' ';
    public kw = new Set([
        'def', 'while', 'for', 'if', 'else', 'continue',
        'break', 'return', 'number', 'bool', 'string',
        'bool', 'fn', 'flex', 'array', 'of'
    ]);
    public ops = new Set([
        '=', '>', '<', '+', '-', '.',
        '/', '*', '&', '|', '!', '%'
    ]);
    public op = (c: string) => this.ops.has(c);
    public sep = (c: string) => c === ';' || c === ',';
    public left = (c: string) => c === '[' || c === '(';
    public right = (c: string) => c === ']' || c === ')';

    // Built-in Lexemes
    public varLexeme: Lexeme = {
        cond: this.vary,
        lex: (src: string, start: number): Token => {
            let end: number = start;
            let value: string = '';
            while (this.vary(src[end]))
                value += src[end++];
            return this.createToken(
                this.kw.has(value) ? 'keyword' : 'variable',
                value, start, end
            );
        },
        require: true
    };
    public numberLexeme: Lexeme = {
        cond: this.digit,
        lex: (src: string, start: number): Token => {
            let end: number = start;
            let value: string = '';
            while (this.digit(src[end]))
                value += src[end++];
            if (src[end] === '.') {
                value += src[end];
                end += 1;
                while (this.digit(src[end]))
                    value += src[end++];
            }
            return this.createToken('number', value, start, end);
        },
        require: true
    };
    public opLexeme: Lexeme = {
        cond: this.op,
        lex: (src: string, start: number): Token => {
            let end: number = start;
            let value: string = '';
            while (this.op(src[end]))
                value += src[end++];
            return this.createToken('operator', value, start, end);
        },
        require: true
    };
    public stringLexeme: Lexeme = {
        cond: this.str,
        lex: (src: string, start: number): Token => {
            let end: number = start + 1;
            let value: string = '';
            while (!this.str(src[end]) && end < src.length)
                value += src[end++];
            if (end === src.length)
                this.lexerError('Incomplete String', start, end);
            return this.createToken('string', value, start, end + 1);
        },
        require: true
    };
    public wsLexeme: Lexeme = {
        cond: this.ws,
        lex: (src: string, start: number): Token => {
            let end: number = start;
            let value: string = '';
            while (this.ws(src[end]))
                value += src[end++];
            return this.createToken('ws', value, start, end);
        },
        require: false
    };
    public separatorLexeme: Lexeme = {
        cond: this.sep,
        lex: (src: string, start: number): Token =>
            this.createToken('separator', src[start], start, ++start),
        require: true
    };
    public leftLexeme: Lexeme = {
        cond: this.left,
        lex: (src: string, start: number): Token =>
            this.createToken(`left:${src[start]}`, src[start], start, ++start),
        require: true
    };
    public rightLexeme: Lexeme = {
        cond: this.right,
        lex: (src: string, start: number): Token =>
            this.createToken(`right:${src[start]}`, src[start], start, ++start),
        require: true
    };
    public otherLexeme: Lexeme = {
        cond: (c: string) => true,
        lex: (src: string, start: number): Token =>
            this.createToken('other', src[start], start, ++start),
        require: false
    };

    // Tokenize
    tokenize(src: string, tokenMap: Array<Lexeme> = [
        this.varLexeme,
        this.numberLexeme,
        this.opLexeme,
        this.stringLexeme,
        this.separatorLexeme,
        this.leftLexeme,
        this.rightLexeme,
        this.wsLexeme,
        this.otherLexeme
    ]): Array<Token> {
        const tokens: Array<Token> = [];
        let i: number = 0;
        while (i < src.length)
            tokenMap.some((toMatch: Lexeme): boolean => {
                if (!toMatch.cond(src[i])) return false;
                const token = toMatch.lex(src, i);
                if (toMatch.require) tokens.push(token);
                i = token.end;
                return true;
            });
        return tokens;
    }

}
