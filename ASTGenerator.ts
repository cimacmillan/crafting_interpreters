import * as fs from "fs";

const OUTPUT_DIR = "src/cpplox/ast/";

function writeFile(filename: string, content: string) {
    const path = OUTPUT_DIR + filename;
    fs.writeFileSync(path, content);
}

enum AstEntryType {
    MEMBER,
    UNION
}

interface AstEntryBase {
    name: string
}

interface MemberAstEntry extends AstEntryBase {
    type: AstEntryType.MEMBER;
    members: Record<string, string>
}

interface UnionAstEntry extends AstEntryBase {
    type: AstEntryType.UNION;
    union: string[]
}

type AstEntry = (MemberAstEntry | UnionAstEntry);

type AST = AstEntry[];

const LOX_AST: AST = [
    {
        type: AstEntryType.MEMBER,
        name: "BinaryExpression",
        members: {
            "left": "Expression",
            "op": "Token",
            "right": "Expression"
        }
    },
    {
        type: AstEntryType.MEMBER,
        name: "GroupingExpression",
        members: {
            "open": "Token",
            "grouped": "Expression",
            "close": "Token"
        }
    },
    {
        type: AstEntryType.MEMBER,
        name: "UnaryExpression",
        members: {
            "expression": "Expression",
            "unary": "Token"
        }
    },
    {
        type: AstEntryType.MEMBER,
        name: "LiteralExpression",
        members: {
            "literal": "Token"
        }
    },
    {
        type: AstEntryType.MEMBER,
        name: "VariableExpression",
        members: {
            "variable": "Token"
        }
    },
    {
        type: AstEntryType.MEMBER,
        name: "LogicalExpression",
        members: {
            left: "Expression",
            op: "Token",
            right: "Expression"
        }
    },
    {
        type: AstEntryType.MEMBER,
        name: "AssignExpression",
        members: {
            "variable": "Token",
            "value": "Expression"
        }
    },
    {
        type: AstEntryType.MEMBER,
        name: "SetExpression",
        members: {
            "variable": "Expression",
            "value": "Expression"
        }
    },
    {
        type: AstEntryType.MEMBER,
        name: "CallExpression",
        members: {
            "callee": "Expression",
            "arguments": "vector<Expression*>"
        }
    },
    {
        type: AstEntryType.MEMBER,
        name: "GetExpression",
        members: {
            "target": "Expression",
            "value": "Token"
        }
    },
    {
        type: AstEntryType.UNION,
        name: "Expression",
        union: ["BinaryExpression", "GroupingExpression", "UnaryExpression", "LiteralExpression", "VariableExpression", "AssignExpression", "LogicalExpression", "CallExpression", "GetExpression", "SetExpression"],
    },
    {
        type: AstEntryType.MEMBER,
        name: "ExpressionStatement",
        members: {
            "expr": "Expression",
            "semicolon": "Token"
        }
    },
    {
        type: AstEntryType.MEMBER,
        name: "PrintStatement",
        members: {
            "print": "Token",
            "expr": "Expression",
            "semicolon": "Token"
        }
    },
    {
        type: AstEntryType.MEMBER,
        name: "BlockStatement",
        members: {
            "block": "vector<struct Declaration*>"
        }
    },
    {
        type: AstEntryType.MEMBER,
        name: "IfStatement",
        members: {
            "condition": "Expression",
            "trueBlock": "Statement",
            "falseBlock": "Statement"
        }
    },
    {
        type: AstEntryType.MEMBER,
        name: "WhileStatement",
        members: {
            "condition": "Expression",
            "block": "Statement"
        }
    },
    {
        type: AstEntryType.MEMBER,
        name: "ReturnStatement",
        members: {
            "expr": "Expression"
        }
    },
    {
        type: AstEntryType.UNION,
        name: "Statement",
        union: ["ExpressionStatement", "PrintStatement", "BlockStatement", "IfStatement", "WhileStatement", "ReturnStatement"]
    },
    {
        type: AstEntryType.MEMBER,
        name: "VarDeclaration",
        members: {
            "var": "Token",
            "identifier": "Token",
            "equals": "Token",
            "expr": "Expression",
            "semi": "Token"
        }
    },
    {
        type: AstEntryType.MEMBER,
        name: "StatementDeclaration",
        members: {
            "statement": "Statement",
            "identifier": "Token",
            "equals": "Token",
            "expr": "Expression",
            "semi": "Token"
        }
    },
    {
        type: AstEntryType.MEMBER,
        name: "FunctionDeclaration",
        members: {
            "identifier": "Token",
            "argIdentifiers": "vector<Token*>",
            "block": "Statement"
        }
    },
    {
        type: AstEntryType.MEMBER,
        name: "ClassDeclaration",
        members: {
            "identifier": "Token",
            "methods": "vector<FunctionDeclaration*>"
        }
    },
    {
        type: AstEntryType.UNION,
        name: "Declaration",
        union: ["VarDeclaration", "StatementDeclaration", "FunctionDeclaration", "ClassDeclaration"]
    },
    {
        type: AstEntryType.MEMBER,
        name: "LoxProgram",
        members: {
            "program": "vector<Declaration*>"
        }
    }
];

function outputMemberType(entry: MemberAstEntry): string {
    let cPlusPlus = ``;
    cPlusPlus += `struct ${entry.name} {\n`
    for (let member in entry.members) {
        cPlusPlus += `\tstruct ${entry.members[member]} *${member};\n`;
    }
    cPlusPlus += "};"
    return cPlusPlus;
}

function outputUnionCreatorFunction(ast: AST, type: string, entry: string) {
    let cPlusPlus = "";
    cPlusPlus += `\tstatic ${type}* as${entry}(${entry} *${entry.toLocaleLowerCase()}) {\n`
    cPlusPlus += `\t\treturn new ${type}({.type=${type}Type::${entry}, .${entry.toLocaleLowerCase()}=${entry.toLocaleLowerCase()}});\n`;
    cPlusPlus += "\t}\n"
    return cPlusPlus;
}

function outputUnionType(ast: AST, entry: UnionAstEntry): string {
    let cPlusPlus = "";
    cPlusPlus += `BETTER_ENUM(${entry.name}Type, char, `;
    cPlusPlus += entry.union.join(",");
    cPlusPlus += ");\n"

    cPlusPlus += `struct ${entry.name} {\n\t${entry.name}Type type;\n\tunion {\n`;

    for (let unionMember of entry.union) {
        cPlusPlus += `\t\t${unionMember} *${unionMember.toLowerCase()};\n`
    }
    cPlusPlus += `\t};\n`

    for (let unionMember of entry.union) {
        cPlusPlus += outputUnionCreatorFunction(ast, entry.name, unionMember);
    }
    

    cPlusPlus += `};`

    return cPlusPlus
}

function outputEntry(ast: AST, entry: AstEntry) {
    switch (entry.type) {
        case AstEntryType.MEMBER:
            return outputMemberType(entry);
        case AstEntryType.UNION:
            return outputUnionType(ast, entry);
    }
}

function outputVisitorMember(ast: AST, entry: MemberAstEntry) {
    return `\tvoid visit(${entry.name} *entry);\n`
}

function outputVisitorUnion(ast: AST, entry: UnionAstEntry) {
    let cpp = '';
    cpp += `\tvoid visit(${entry.name} *entry) {\n`;
    cpp += `\t\tswitch(entry->type) {\n`
    for (let name of entry.union) {
        cpp += `\t\t\tcase ${entry.name}Type::${name}:\n`
        cpp += `\t\t\t\tthis->visit(entry->${name.toLowerCase()});\n`
        cpp += `\t\t\t\tbreak;\n`
    }
    cpp += `\t\t}\n`
    cpp += `\t}\n`;
    return cpp;
}

function outputVisitor(ast: AST) {
    let cpp = '';
    cpp += `class AstVisitor {\n`;
    cpp += `public:\n`
    for (let entry of ast) {
        switch (entry.type) {
            case AstEntryType.MEMBER:
                cpp += outputVisitorMember(ast, entry);
                break;
            case AstEntryType.UNION:
                cpp += outputVisitorUnion(ast, entry);
                break;
        }
    }
    cpp += `};\n`
    return cpp;
}

function outputAst(ast: AST) {
    let source = `// This is generated by scripts/ASTGenerator.ts\n#pragma once\n#include <enum.h>\n#include <vector>\nusing namespace std;\n\n`
    for (let entry of ast) {
        source += outputEntry(ast, entry) + "\n";
    }

    source += outputVisitor(ast);
    writeFile("ast.h", source);
}


outputAst(LOX_AST);

