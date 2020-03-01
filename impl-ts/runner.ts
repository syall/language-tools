#!/usr/local/bin/deno --allow-read

// @ts-ignore
import { Lexer } from './lex.ts';
// @ts-ignore
import { Parser } from './parse.ts';

// Source Code
const decoder = new TextDecoder('utf-8');
// @ts-ignore
const data = Deno.readFileSync('./examples/program.sm');
const program: string = decoder.decode(data);

// Lexer
const tokens = new Lexer().tokenize(program);
console.log('Tokens:');
console.table(tokens);
console.log();

// Parser
const { children } = new Parser().run(tokens);
const options = ['type', 'name', 'value', 'children', 'args'];
const separator = '---';
console.log('Tree:');
console.log(JSON.stringify(children, options, separator));
console.log();
