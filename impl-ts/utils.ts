// Types
export type Token = {
    type: string;
    value: any;
    start: number;
    end: number;
};

export type Lexeme = {
    cond: (c: string) => boolean;
    lex: (src: string, i: number) => Token;
    require: boolean;
};

export type Tree = {
    type: string;
    name?: string;
    value?: any;
    children?: Array<Tree>;
    args?: Array<Tree>;
    start: number;
    end: number;
};

export type Result = {
    success?: any;
    failure?: any;
}
