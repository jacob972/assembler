# assembler

ממן 14

### Milestones

| Description                                           | Due By     |
| ----------------------------------------------------- | ---------- |
| Break each input line in each input file into tokens. | 2020-08-07 |
| Implement and populate the symbol table.              | 2020-08-12 |
| Generate the machine code.                            | 2020-08-18 |
| Fix problems and submit.                              | 2020-08-23 |

### Expected Output

#### 1st Milestone

```scheme
; file ps.as
( (DIRECTIVE 'entry') (LABELREF 'LIST') )
( (DIRECTIVE 'extern') (LABELREF 'W') )
( (LABELDEF 'MAIN') (INSTRUCTION 'add') (REGISTER 3) (COMMA) (LABELREF 'LIST') )
( (LABELDEF 'LOOP') (INSTRUCTION 'prn') (IMMEDIATE 48) )
( (INSTRUCTION 'lea') (LABELREF 'W') (COMMA) (REGISTER 6) )
( (INSTRUCTION 'inc') (REGISTER 6) )
( (INSTRUCTION 'mov') (REGISTER 3) (COMMA) (LABELREF 'K') )
( (INSTRUCTION 'sub') (REGISTER 1) (COMMA) (REGISTER 4) )
( (INSTRUCTION 'bne') (LABELREF 'END') )
( (INSTRUCTION 'cmp') (LABELREF 'K') (COMMA) (IMMEDIATE -6) )
( (INSTRUCTION 'bne') (RELATIVE 'END') )
( (INSTRUCTION 'dec') (LABELREF 'W') )
( (DIRECTIVE 'entry') (LABELREF 'MAIN') )
( (INSTRUCTION 'jmp') (RELATIVE 'LOOP') )
( (INSTRUCTION 'add') (LABELREF 'L3') (COMMA) (LABELREF 'L3') )
( (LABELDEF 'END') (INSTRUCTION 'stop') )
( (LABELDEF 'STR') (DIRECTIVE 'string') (STRING "abcd") )
( (LABELDEF 'LIST') (DIRECTIVE 'data') (NUMBER 6) (COMMA) (NUMBER -9) )
( (DIRECTIVE 'data') (NUMBER -100) )
( (LABELDEF 'K') (DIRECTIVE 'data') (NUMBER 31) )
( (DIRECTIVE 'extern') (LABELREF 'L3') )
```

#### 2nd Milestone

```scheme
( ('MAIN' 0 CODE) ('LOOP' 0 CODE) ('END' 0 CODE) ('STR' 0 DATA) ('LIST' 0 DATA) ('K' 0 DATA) )
```