// @ts-ignore
import { Token, Tree, Result } from './utils.ts';

export class Parser {

    // Index
    public i: number = 0;
    public start: number = 0;
    public tokens: Array<Token> = [];

    // Utilities
    public parserError = (msg: string, end: number) => {
        console.error(`Error at ${this.start} to ${end}: ${msg}`);
    };

    // Helpers
    public peek(i?: number): Token {
        return this.tokens[i !== undefined ? this.i + i : this.i];
    }
    public advance(): Token {
        const cur: Token = this.peek();
        this.i += 1;
        return cur;
    }

    // Parse
    public run(tokens: Array<Token>): Tree {
        const state: Result = {
            success: {
                type: 'program',
                children: [],
                start: this.start,
                end: -1
            }
        };
        this.tokens = tokens;
        while (this.i < tokens.length) {
            const { success, failure } = this.parse(this.statement);
            if (failure !== undefined) {
                this.parserError(failure.msg, failure.end);
                // @ts-ignore
                Deno.exit(1);
            }
            state.success.children.push(success);
        }
        return state.success;
    }

    public parse(production): Result {
        for (const rule of production.content) {
            const { success, failure } = this.match(rule);
            if (success !== undefined)
                return { success, failure };
        }
        return {
            failure: {
                msg: `Could not parse ${production.name}`,
                end: this.tokens[this.i].end
            }
        };
    }

    public match(rule): Result {
        return {
            failure: {
                msg: `Match failed: ${rule.join(' ')}`,
                end: this.tokens[this.i].end
            }
        };
    }

    public statement = {
        name: 'statement',
        content: [
            ['expr'],
            ['def', 'id', ':', 'type', '=', 'expr']
        ]
    };

}
