from modules.instruction import Instruction

#ExitCodes
class ErrNum:
    OK = 0
    BAD_PARAM = 10
    FILE_IN_OPEN_ERR = 11
    FILE_OUT_OPEN_ERR = 12
    INNER_ERR = 99
    XML_FORMAT_ERR = 31
    XML_UNEXPECTED_STRUCT = 32
    INT_SEM_INPUT = 52
    INT_BAD_OPERANDS_TYPE = 53
    INT_NON_EXISTING_VAR = 54
    INT_NON_EXISTING_FRAME = 55
    INT_NON_EXISTING_VALUE = 56 
    INT_BAD_OPERAND_VALUE = 57  #deleni nulou, spatnIa navratova hodnota instrukce EXIT
    INT_STRING_ERR = 58



#symb,labe,var,var_type definition
symb = {"string":r"^([^\s#\\]|\\[0-9]{3})*$","int":r"^(\+|\-)?[0-9]+$","bool":r"^(true|false)$","nil":r"^nil$","var":r"^(GF|LF|TF)@(_|\-|$|%|\*|!|\?|[a-zA-Z])+[^\s]*$"}
label = {"label":r"^(_|\-|$|%|\*|!|\?|[a-zA-Z])+[^\s]*$"}
var = {"var":r"^(GF|LF|TF)@(_|\-|$|%|\*|!|\?|[a-zA-Z])+[^\s]*$"}
var_type = {"type":r"^(bool|int|nil|string)$"}

#contains information about opcode and it's allowed arguments
#indexable by opcode
allowed_instructions = {"MOVE":[var,symb],"CREATEFRAME":[],"PUSHFRAME":[],"POPFRAME":[],"DEFVAR":[var],"CALL":[label],"RETURN":[],"PUSHS":[symb],"POPS":[var]\
            ,"ADD":[var,symb,symb],"SUB":[var,symb,symb],"MUL":[var,symb,symb],"IDIV":[var,symb,symb],"LT":[var,symb,symb],"GT":[var,symb,symb]\
            ,"EQ":[var,symb,symb],"AND":[var,symb,symb],"OR":[var,symb,symb],"NOT":[var,symb],"INT2CHAR":[var,symb],"STRI2INT":[var,symb,symb]\
            ,"READ":[var,var_type],"WRITE":[symb],"CONCAT":[var,symb,symb],"STRLEN":[var,symb],"GETCHAR":[var,symb,symb],"SETCHAR":[var,symb,symb]\
            ,"TYPE":[var,symb],"LABEL":[label],"JUMP":[label],"JUMPIFEQ":[label,symb,symb],"JUMPIFNEQ":[label,symb,symb],"EXIT":[symb],"DPRINT":[symb],"BREAK":[]}