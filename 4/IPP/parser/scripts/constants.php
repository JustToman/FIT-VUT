<?php


/**
 * Class containing Token types,and REGEX_ARRAY.
 * Each Token has assigned number.
 * This number is index to REGEX_ARRAY too (Index to regex belonging to token)
 * 
 */
class TokenConstants
{
    const CONSTANT = 0;
    const ID = 1;
    const HEADER = 2;
    const KW_MOVE = 3;
    const KW_CREATEFRAME = 4;
    const KW_PUSHFRAME = 5;
    const KW_POPFRAME = 6;
    const KW_DEFVAR = 7;
    const KW_CALL = 8;
    const KW_RETURN = 9;
    const KW_PUSHS = 10;
    const KW_POPS = 11;
    const KW_ADD = 12;
    const KW_SUB = 13;
    const KW_MUL = 14;
    const KW_IDIV = 15;
    const KW_LT = 16;
    const KW_GT = 17;
    const KW_EQ = 18;
    const KW_AND = 19;
    const KW_OR = 20;
    const KW_NOT = 21;
    const KW_INT2CHAR = 22;
    const KW_STR2INT = 23;
    const KW_READ = 24;
    const KW_WRITE = 25;
    const KW_CONCAT = 26;
    const KW_STRLEN = 27;
    const KW_GETCHAR = 28;
    const KW_SETCHAR = 29;
    const KW_TYPE = 30;
    const KW_LABEL = 31;
    const KW_JUMP = 32;
    const KW_JUMPIFEQ = 33;
    const KW_JUMPIFNEQ = 34;
    const KW_EXIT = 35;
    const KW_DPRINT = 36;
    const KW_BREAK = 37;
    const TYPE = 38;
    const LABEL = 39;
    const EOF = 40;
    const LEX_ERR = 41;

    const REGEX_ARR = array('/\Astring@([^\s\\\\#]|\\\\[0-9]{3})*\z|^bool@true\z|^bool@false\z|^int@(\\+|-)?[0-9]+\z|^nil@nil\z/u'
                            ,"/^(LF|GF|TF)@(_|-|\\*|\\$|&|%|!|\\?|[a-zA-Z])+((_|-|\\*|\\$|&|%|!|\\?|[a-zA-Z])|[0-9])*\z/"
                            ,"/^\.ippcode21\z/i","/^MOVE\z/i","/^CREATEFRAME\z/i","/^PUSHFRAME\z/i"
                            ,"/^POPFRAME\z/i","/^DEFVAR\z/i","/^CALL\z/i","/^RETURN\z/i","/^PUSHS\z/i"
                            ,"/^POPS\z/i","/^ADD\z/i","/^SUB\z/i","/^MUL\z/i","/^IDIV\z/i","/^LT\z/i"
                            ,"/^GT\z/i","/^EQ\z/i","/^AND\z/i","/^OR\z/i","/^NOT\z/i","/^INT2CHAR\z/i"
                            ,"/^STRI2INT\z/i","/^READ\z/i","/^WRITE\z/i","/^CONCAT\z/i","/^STRLEN\z/i"
                            ,"/^GETCHAR\z/i","/^SETCHAR\z/i","/^TYPE\z/i","/^LABEL\z/i","/^JUMP\z/i"
                            ,"/^JUMPIFEQ\z/i","/^JUMPIFNEQ\z/i","/^EXIT\z/i","/^DPRINT\z/i","/^BREAK\z/i"
                            ,"/^(bool|string|int)\z/",'/^(_|-|\\*|\\$|&|%|!|\\?|[a-zA-Z])+((_|-|\\*|\\$|&|%|!|\\?|[a-zA-Z])|[0-9])*\z/i');
}

/**
 * Class containing Error codes constants
 */
class ErrNums
{
    const MISSING_PARAMETERS = 10;
    const FILE_INPUT_ERROR = 11;
    const FILE_OUTPUT_ERROR = 12;
    const MISSING_HEADER = 21;
    const BAD_OP_CODE = 22;
    const OTHERS = 23;
    const INNER_ERROR = 99;
}

/**
 * Class containing array named GRAMMAR, that holds Syntax rules
 * of IPPcode21
 */
class Grammar
{
    const GRAMMAR = array(
    array(TokenConstants::HEADER),
    array(TokenConstants::KW_MOVE,TokenConstants::ID,TokenConstants::CONSTANT),
    array(TokenConstants::KW_MOVE,TokenConstants::ID,TokenConstants::ID),
    array(TokenConstants::KW_CREATEFRAME),
    array(TokenConstants::KW_PUSHFRAME),
    array(TokenConstants::KW_POPFRAME),
    array(TokenConstants::KW_DEFVAR,TokenConstants::ID),
    array(TokenConstants::KW_CALL,TokenConstants::LABEL),
    array(TokenConstants::KW_RETURN),
    array(TokenConstants::KW_PUSHS,TokenConstants::CONSTANT),
    array(TokenConstants::KW_PUSHS,TokenConstants::ID),
    array(TokenConstants::KW_POPS,TokenConstants::ID),
    array(TokenConstants::KW_ADD,TokenConstants::ID,TokenConstants::CONSTANT,TokenConstants::CONSTANT),
    array(TokenConstants::KW_ADD,TokenConstants::ID,TokenConstants::CONSTANT,TokenConstants::ID),
    array(TokenConstants::KW_ADD,TokenConstants::ID,TokenConstants::ID,TokenConstants::CONSTANT),
    array(TokenConstants::KW_ADD,TokenConstants::ID,TokenConstants::ID,TokenConstants::ID),
    array(TokenConstants::KW_SUB,TokenConstants::ID,TokenConstants::CONSTANT,TokenConstants::CONSTANT),
    array(TokenConstants::KW_SUB,TokenConstants::ID,TokenConstants::CONSTANT,TokenConstants::ID),
    array(TokenConstants::KW_SUB,TokenConstants::ID,TokenConstants::ID,TokenConstants::CONSTANT),
    array(TokenConstants::KW_SUB,TokenConstants::ID,TokenConstants::ID,TokenConstants::ID),
    array(TokenConstants::KW_MUL,TokenConstants::ID,TokenConstants::CONSTANT,TokenConstants::CONSTANT),
    array(TokenConstants::KW_MUL,TokenConstants::ID,TokenConstants::CONSTANT,TokenConstants::ID),
    array(TokenConstants::KW_MUL,TokenConstants::ID,TokenConstants::ID,TokenConstants::CONSTANT),
    array(TokenConstants::KW_MUL,TokenConstants::ID,TokenConstants::ID,TokenConstants::ID),
    array(TokenConstants::KW_IDIV,TokenConstants::ID,TokenConstants::CONSTANT,TokenConstants::CONSTANT),
    array(TokenConstants::KW_IDIV,TokenConstants::ID,TokenConstants::CONSTANT,TokenConstants::ID),
    array(TokenConstants::KW_IDIV,TokenConstants::ID,TokenConstants::ID,TokenConstants::CONSTANT),
    array(TokenConstants::KW_IDIV,TokenConstants::ID,TokenConstants::ID,TokenConstants::ID),
    array(TokenConstants::KW_LT,TokenConstants::ID,TokenConstants::CONSTANT,TokenConstants::CONSTANT),
    array(TokenConstants::KW_LT,TokenConstants::ID,TokenConstants::CONSTANT,TokenConstants::ID),
    array(TokenConstants::KW_LT,TokenConstants::ID,TokenConstants::ID,TokenConstants::CONSTANT),
    array(TokenConstants::KW_LT,TokenConstants::ID,TokenConstants::ID,TokenConstants::ID),
    array(TokenConstants::KW_GT,TokenConstants::ID,TokenConstants::CONSTANT,TokenConstants::CONSTANT),
    array(TokenConstants::KW_GT,TokenConstants::ID,TokenConstants::CONSTANT,TokenConstants::ID),
    array(TokenConstants::KW_GT,TokenConstants::ID,TokenConstants::ID,TokenConstants::CONSTANT),
    array(TokenConstants::KW_GT,TokenConstants::ID,TokenConstants::ID,TokenConstants::ID),
    array(TokenConstants::KW_EQ,TokenConstants::ID,TokenConstants::CONSTANT,TokenConstants::CONSTANT),
    array(TokenConstants::KW_EQ,TokenConstants::ID,TokenConstants::CONSTANT,TokenConstants::ID),
    array(TokenConstants::KW_EQ,TokenConstants::ID,TokenConstants::ID,TokenConstants::CONSTANT),
    array(TokenConstants::KW_EQ,TokenConstants::ID,TokenConstants::ID,TokenConstants::ID),
    array(TokenConstants::KW_AND,TokenConstants::ID,TokenConstants::CONSTANT,TokenConstants::CONSTANT),
    array(TokenConstants::KW_AND,TokenConstants::ID,TokenConstants::CONSTANT,TokenConstants::ID),
    array(TokenConstants::KW_AND,TokenConstants::ID,TokenConstants::ID,TokenConstants::CONSTANT),
    array(TokenConstants::KW_AND,TokenConstants::ID,TokenConstants::ID,TokenConstants::ID),
    array(TokenConstants::KW_OR,TokenConstants::ID,TokenConstants::CONSTANT,TokenConstants::CONSTANT),
    array(TokenConstants::KW_OR,TokenConstants::ID,TokenConstants::CONSTANT,TokenConstants::ID),
    array(TokenConstants::KW_OR,TokenConstants::ID,TokenConstants::ID,TokenConstants::CONSTANT),
    array(TokenConstants::KW_OR,TokenConstants::ID,TokenConstants::ID,TokenConstants::ID),
    array(TokenConstants::KW_NOT,TokenConstants::ID,TokenConstants::CONSTANT),
    array(TokenConstants::KW_NOT,TokenConstants::ID,TokenConstants::ID),
    array(TokenConstants::KW_INT2CHAR, TokenConstants::ID, TokenConstants::CONSTANT),
    array(TokenConstants::KW_INT2CHAR, TokenConstants::ID, TokenConstants::ID),
    array(TokenConstants::KW_STR2INT,TokenConstants::ID,TokenConstants::CONSTANT,TokenConstants::CONSTANT),
    array(TokenConstants::KW_STR2INT,TokenConstants::ID,TokenConstants::CONSTANT,TokenConstants::ID),
    array(TokenConstants::KW_STR2INT,TokenConstants::ID,TokenConstants::ID,TokenConstants::CONSTANT),
    array(TokenConstants::KW_STR2INT,TokenConstants::ID,TokenConstants::ID,TokenConstants::ID),
    array(TokenConstants::KW_READ,TokenConstants::ID,TokenConstants::TYPE),
    array(TokenConstants::KW_WRITE,TokenConstants::CONSTANT),
    array(TokenConstants::KW_WRITE,TokenConstants::ID),
    array(TokenConstants::KW_CONCAT,TokenConstants::ID,TokenConstants::CONSTANT,TokenConstants::CONSTANT),
    array(TokenConstants::KW_CONCAT,TokenConstants::ID,TokenConstants::CONSTANT,TokenConstants::ID),
    array(TokenConstants::KW_CONCAT,TokenConstants::ID,TokenConstants::ID,TokenConstants::CONSTANT),
    array(TokenConstants::KW_CONCAT,TokenConstants::ID,TokenConstants::ID,TokenConstants::ID),
    array(TokenConstants::KW_STRLEN, TokenConstants::ID,TokenConstants::ID),
    array(TokenConstants::KW_STRLEN, TokenConstants::ID,TokenConstants::CONSTANT),
    array(TokenConstants::KW_GETCHAR,TokenConstants::ID,TokenConstants::CONSTANT,TokenConstants::CONSTANT),
    array(TokenConstants::KW_GETCHAR,TokenConstants::ID,TokenConstants::CONSTANT,TokenConstants::ID),
    array(TokenConstants::KW_GETCHAR,TokenConstants::ID,TokenConstants::ID,TokenConstants::CONSTANT),
    array(TokenConstants::KW_GETCHAR,TokenConstants::ID,TokenConstants::ID,TokenConstants::ID),
    array(TokenConstants::KW_SETCHAR,TokenConstants::ID,TokenConstants::CONSTANT,TokenConstants::CONSTANT),
    array(TokenConstants::KW_SETCHAR,TokenConstants::ID,TokenConstants::CONSTANT,TokenConstants::ID),
    array(TokenConstants::KW_SETCHAR,TokenConstants::ID,TokenConstants::ID,TokenConstants::CONSTANT),
    array(TokenConstants::KW_SETCHAR,TokenConstants::ID,TokenConstants::ID,TokenConstants::ID),
    array(TokenConstants::KW_TYPE,TokenConstants::ID,TokenConstants::CONSTANT),
    array(TokenConstants::KW_TYPE,TokenConstants::ID,TokenConstants::ID),
    array(TokenConstants::KW_LABEL, TokenConstants::LABEL),
    array(TokenConstants::KW_JUMP, TokenConstants::LABEL),
    array(TokenConstants::KW_JUMPIFEQ,TokenConstants::LABEL,TokenConstants::CONSTANT,TokenConstants::CONSTANT),
    array(TokenConstants::KW_JUMPIFEQ,TokenConstants::LABEL,TokenConstants::CONSTANT,TokenConstants::ID),
    array(TokenConstants::KW_JUMPIFEQ,TokenConstants::LABEL,TokenConstants::ID,TokenConstants::CONSTANT),
    array(TokenConstants::KW_JUMPIFEQ,TokenConstants::LABEL,TokenConstants::ID,TokenConstants::ID),
    array(TokenConstants::KW_JUMPIFNEQ,TokenConstants::LABEL,TokenConstants::CONSTANT,TokenConstants::CONSTANT),
    array(TokenConstants::KW_JUMPIFNEQ,TokenConstants::LABEL,TokenConstants::CONSTANT,TokenConstants::ID),
    array(TokenConstants::KW_JUMPIFNEQ,TokenConstants::LABEL,TokenConstants::ID,TokenConstants::CONSTANT),
    array(TokenConstants::KW_JUMPIFNEQ,TokenConstants::LABEL,TokenConstants::ID,TokenConstants::ID),
    array(TokenConstants::KW_EXIT,TokenConstants::CONSTANT),
    array(TokenConstants::KW_EXIT,TokenConstants::ID),
    array(TokenConstants::KW_DPRINT,TokenConstants::CONSTANT),
    array(TokenConstants::KW_DPRINT, TokenConstants::ID),
    array(TokenConstants::KW_BREAK),
    );

}

?>