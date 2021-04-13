import modules.constants as constants
import sys
import re
from modules.instruction import Instruction

def parse(instructions:list):
    """This function provides lexical and syntax analysis for ippCode21 xml form.
    If code is lexically or syntantically wrong, it exits with code 32 (XML_UNEXPECTED_STRUCT)

    Args:
        instructions (list):  List of all Instruction instances defining src code
    Return:
        None
    """
    for inst in instructions:
        #If opcode doesn't exist exits
        if inst.opcode not in constants.allowed_instructions:
            sys.exit(constants.ErrNum.XML_UNEXPECTED_STRUCT)
        #If there are different number of args with given opcode than it should be
        if len(inst.args) != len(constants.allowed_instructions[inst.opcode]):
            sys.exit(constants.ErrNum.XML_UNEXPECTED_STRUCT)
        #Checks if instructions have right types and are lexically right.
        for i in range(len(constants.allowed_instructions[inst.opcode])):
            if inst.args[i]["type"] not in constants.allowed_instructions[inst.opcode][i].keys():
                sys.exit(constants.ErrNum.XML_UNEXPECTED_STRUCT)
            if re.match(constants.allowed_instructions[inst.opcode][i][inst.args[i]["type"]],inst.args[i]["value"]) is None:
                sys.exit(constants.ErrNum.XML_UNEXPECTED_STRUCT)

        
        

