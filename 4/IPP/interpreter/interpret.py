'''
Author: Roman Popelka(xpopel24)
'''
import modules.args_to_instruction as args_to_instruction
from modules.instruction import Instruction
from modules.parser import parse
import sys
import re
from modules.constants import ErrNum
from pprint import pprint
import codecs



class Variable:
    """
    Class representing variable in interpreter

    Attributes:
        name     (str): name of variable
        var_type (str): type of variable
        value    (str|Nil|bool|int): value of variable depending on var_type
    """
    
    def __init__(self,name,var_type,value):
        self.name = name
        self.var_type = var_type
        self.value = value
    
    def __eq__(self,other):
        """Overriding _eq_ to match Variable Instances with same name
        """
        if not isinstance(other,Variable):
            return False
        
        return self.name == other.name

class Nil:
    """Class representing ippCode21 nil variable

    Attributes:
        val (string): value of nil (always empty string (""))
    """
    def _init__(self):
        self.val = ""

    def __eq__(self,other):
        if isinstance(other,Nil) == False:
            return False
        else:
            return True

class stack(list):
    """Class representing stack
    It's list's descendant enriched by stack's is_empty() and top() method
    """
    def top(self):
        """
        Returns element on top of the stack
        """
        return self[-1]
    def is_empty(self):
        """
        If stack is empty returns true
        """
        return len(self) == 0

#Creating global_frame and data_stack
global_frame = list()
local_frame_stack = stack()
temporary_frame = None #None if undefined, list if defined
data_stack = stack()
call_stack = stack()
read_index = 0

def escape_sequence(val:str):
    """Evaluating escape sequence in string to create escaped character
    Evaluates only escape sequence with format: \\xxx where x belongs to <1:9>
    
    Args:
        val (str): string constant that could contai escape sequence
    
    Returns:
        string: string with evaluated escape sequence
    """

    new_val = ""
    #Splits string by escape character (backslash)
    #and loops through it's elements
    for value in val.split("\\"):
        #If element of list starts with 3 numbers, there was an escape sequence
        #So it's evaluated
        #If not, value of element stays the same
        #this edited value is concatenated to "new_value" which will be returned
        if re.match(r'^[0-9]{3}',value) is not None:
            new_val += chr(int(value[:3]))+value[3:]
        else:
            new_val += value
    return new_val

def change_strings(instructions:list):
    """Evaluates escape sequences in all instructions arguments (if their type is string)

    Args:
        instructions (list):  List of all Instruction instances defining src code

    Returns:
        list: list of instructions with evaluated escape sequences in string args
    """
    for i in range(len(instructions)):
        for j in range(len(instructions[i].args)):
            if instructions[i].args[j]["type"] == "string":
                instructions[i].args[j]["value"] = escape_sequence(instructions[i].args[j]["value"])
    return instructions

def push_to_frame(frame:str,var:Variable):

    if frame == "GF":
        if var_already_exists(global_frame,var.name):
            sys.exit(ErrNum.INT_SEM_INPUT)
        global_frame.append(var)
    if frame == "LF":
        #checks if any frame is create
        if len(local_frame_stack) == 0:
            sys.exit(ErrNum.INT_NON_EXISTING_FRAME)
        #Check if variable exists in current frame
        if var_already_exists(local_frame_stack.top(),var.name):
            sys.exit(ErrNum.INT_SEM_INPUT)
        #Adds variable to frame
        local_frame_stack.top().append(var)

    if frame == "TF":
        if temporary_frame == None:
            sys.exit(ErrNum.INT_NON_EXISTING_FRAME)
        if var_already_exists(temporary_frame,var.name):
            sys.exit(ErrNum.INT_SEM_INPUT)
        
        temporary_frame.append(var)
        

def var_already_exists(frame:str,var_name:str):
    if frame == "GF":
        frame = global_frame
    if frame == "LF":
        frame = local_frame_stack.top()
    if frame == "TF":
        frame = temporary_frame

    for var in frame:
        if var.name == var_name:
            return True
    return False

def get_frame_and_var_name(var_name:str):
    return {'frame':var_name[:2],"name":var_name[3:]}    

def frame_exists(frame:str):
    if frame == "GF":
        return True
    if frame == "TF":
        return temporary_frame is not None
    if frame == "LF":
        return local_frame_stack.is_empty() == False

def retype_str_val(type:str,val):
    if type == "bool":
        if val == "true":
            return True
        if val == "false":
            return False
    if type == "int":
        return int(val)
    if type == "nil":
        return Nil()
    return val


def edit_var(var_name:str,var_type:str,value):
    check_var_validity(var_name)
    var_info = get_frame_and_var_name(var_name)
    if var_info["frame"] == "GF":
        for i in range(len(global_frame)):
            if global_frame[i].name == var_info["name"]:
                global_frame[i].var_type = var_type
                global_frame[i].value = retype_str_val(var_type,value)
    if var_info["frame"] == "TF":
        for i in range(len(temporary_frame)):
            if temporary_frame[i].name == var_info["name"]:
                temporary_frame[i].var_type = var_type
                temporary_frame[i].value = retype_str_val(var_type,value)
    if var_info["frame"] == "LF":
        for i in range(len(local_frame_stack.top())):
            if local_frame_stack.top()[i].name == var_info["name"]:
                local_frame_stack.top()[i].var_type = var_type
                local_frame_stack.top()[i].value = retype_str_val(var_type,value)

def get_var(var_name:str):
    check_var_validity(var_name)
    var_info = get_frame_and_var_name(var_name)
    if var_info["frame"] == "GF":
        frame = global_frame
    if var_info["frame"] == "LF":
        frame = local_frame_stack.top()
    if var_info["frame"] == "TF":
        frame = temporary_frame
    
    for var in frame:
        if var.name == var_info["name"]:
            return var

def check_var_validity(var_name:str):
    var_info = get_frame_and_var_name(var_name)
    if frame_exists(var_info["frame"]) == False:
        sys.exit(ErrNum.INT_NON_EXISTING_FRAME)
    if var_already_exists(var_info["frame"],var_info["name"]) == False:
        sys.exit(ErrNum.INT_NON_EXISTING_VAR)

def is_readable(var:Variable):
    return var.value != None


def find_label_index(label,labels:dict):
    if label in labels.keys():
        return labels[label]
    sys.exit(ErrNum.INT_SEM_INPUT)


def get_type(value):
    if isinstance(value,bool):
        return "bool"
    elif isinstance(value,int):
        return "int"
    elif isinstance(value,str):
        return "string"
    elif isinstance(value,Nil):
        return "nil"

def get_value_n_type(arg):
    if arg["type"] == "var":
        var = get_var(arg["value"])
        if is_readable(var) == False:
            sys.exit(ErrNum.INT_NON_EXISTING_VALUE)
        return {"value":var.value,"type":var.var_type}
    else:
        return {"value":retype_str_val(arg["type"],arg["value"]),"type":arg["type"]}
def get_type_from_arg(arg):
    if arg["type"] == "var":
        var = get_var(arg["value"])
        return var.var_type
    else:
        return arg["type"]
def get_all_labels(instructions):
    labels = dict()
    
    for i in range(len(instructions)):
        if instructions[i].opcode == 'LABEL':
            if instructions[i].args[0]["value"] in labels.keys():
                sys.exit(ErrNum.INT_SEM_INPUT)
            labels[instructions[i].args[0]["value"]] = i
    
    return labels




    
 


if __name__ == "__main__":
    arg_data = args_to_instruction.arg_parse()
    args_to_instruction.check_xml_validity(arg_data["source"])
    instructions = args_to_instruction.xml_to_inst(arg_data["source"])
    instructions = args_to_instruction.sort_instructions(instructions)
    instructions = args_to_instruction.sort_args(instructions)
    args_to_instruction.check_args_orders(instructions)
    parse(instructions)
    
    instructions = change_strings(instructions)

    labels = get_all_labels(instructions)


    i = 0
    while(i < len(instructions)):
        inst = instructions[i]
        if inst.opcode == "DEFVAR":
            var_info = get_frame_and_var_name(inst.args[0]["value"])
            push_to_frame(var_info["frame"],Variable(var_info["name"],None,None))
        elif inst.opcode == "CREATEFRAME":
            temporary_frame = list()
        elif inst.opcode == "PUSHFRAME":
            if temporary_frame == None:
                sys.exit(ErrNum.INT_NON_EXISTING_FRAME)
            local_frame_stack.append(temporary_frame)
            temporary_frame = None
        elif inst.opcode == "POPFRAME":
            if local_frame_stack.is_empty():
                sys.exit(ErrNum.INT_NON_EXISTING_FRAME)
            temporary_frame = local_frame_stack.pop()
        elif inst.opcode == "MOVE":
            if inst.args[0]["type"] != "var":
                sys.exit(ErrNum.INT_BAD_OPERANDS_TYPE)
            check_var_validity(inst.args[0]["value"]) 
            if inst.args[1]["type"] == "var":
                var = get_var(inst.args[1]["value"])
                if is_readable(var) == False:
                   sys.exit(ErrNum.INT_NON_EXISTING_VALUE)
                edit_var(inst.args[0]["value"],var.var_type,var.value)
            else:
               edit_var(inst.args[0]["value"],inst.args[1]["type"],retype_str_val(inst.args[1]["type"],inst.args[1]["value"]))
        elif inst.opcode == "CALL":
            call_stack.append(i+1)
            label = inst.args[0]["value"]
            start_index = find_label_index(label,labels)
            i = start_index + 1 # + 1 protoze label by se vytvoril znovu coz je nerozumne
            continue
        elif inst.opcode == "RETURN":
            if call_stack.is_empty():
                sys.exit(ErrNum.INT_NON_EXISTING_VALUE)
            i = call_stack.pop()
            continue
        elif inst.opcode == "PUSHS":
            if inst.args[0]["type"] == "var":
                var = get_var(inst.args[0]["value"])
                if is_readable(var) == False:
                    sys.exit(ErrNum.INT_NON_EXISTING_VALUE)
                data_stack.append(var.value)
            else:
                data_stack.append(retype_str_val(inst.args[0]["type"],inst.args[0]["value"]))
        elif inst.opcode == "POPS":
            if data_stack.is_empty():
                sys.exit(ErrNum.INT_NON_EXISTING_VALUE)
            data = data_stack.pop()
            edit_var(inst.args[0]["value"],get_type(data),data)
        elif inst.opcode == "ADD":
            #Check if var exists
            val1 = get_value_n_type(inst.args[1])
            val2 = get_value_n_type(inst.args[2])
            if val1["type"] != "int" or val2["type"] != "int":
                sys.exit(ErrNum.INT_BAD_OPERANDS_TYPE)
            
            edit_var(inst.args[0]["value"],"int",int(val1["value"]) + int(val2["value"]))
        elif inst.opcode == "SUB":
            val1 = get_value_n_type(inst.args[1])
            val2 = get_value_n_type(inst.args[2])
            if val1["type"] != "int" or val2["type"] != "int":
                sys.exit(ErrNum.INT_BAD_OPERANDS_TYPE)
            
            edit_var(inst.args[0]["value"],"int",int(val1["value"]) - int(val2["value"]))
        elif inst.opcode == "MUL":
            val1 = get_value_n_type(inst.args[1])
            val2 = get_value_n_type(inst.args[2])
            if val1["type"] != "int" or val2["type"] != "int":
                sys.exit(ErrNum.INT_BAD_OPERANDS_TYPE)
            
            edit_var(inst.args[0]["value"],"int",int(val1["value"]) * int(val2["value"]))
        elif inst.opcode == "IDIV":
            val1 = get_value_n_type(inst.args[1])
            val2 = get_value_n_type(inst.args[2])
            if val1["type"] != "int" or val2["type"] != "int":
                sys.exit(ErrNum.INT_BAD_OPERANDS_TYPE)
            if int(val2["value"]) == 0:
                sys.exit(ErrNum.INT_BAD_OPERAND_VALUE)

            edit_var(inst.args[0]["value"],"int",val1["value"] // val2["value"])
        elif inst.opcode == "LT":
            val1 = get_value_n_type(inst.args[1])
            val2 = get_value_n_type(inst.args[2])

            if val1["type"] != val2["type"] or val1["type"] == "nil" or val2["type"] == "nil":
                sys.exit(ErrNum.INT_BAD_OPERANDS_TYPE)
            
            comparison_output = retype_str_val(val1["type"],val1["value"]) < retype_str_val(val2["type"],val2["value"])
            edit_var(inst.args[0]["value"],"bool",comparison_output)
        elif inst.opcode == "GT":
            val1 = get_value_n_type(inst.args[1])
            val2 = get_value_n_type(inst.args[2])

            if val1["type"] != val2["type"] or (val1["type"] == "nil" or val2["type"] == "nil"):
                sys.exit(ErrNum.INT_BAD_OPERANDS_TYPE)
            
            comparison_output = retype_str_val(val1["type"],val1["value"]) > retype_str_val(val2["type"],val2["value"])
            edit_var(inst.args[0]["value"],"bool",comparison_output)
        elif inst.opcode == "EQ":
            val1 = get_value_n_type(inst.args[1])
            val2 = get_value_n_type(inst.args[2])

            if val1["type"] != val2["type"] and val1["type"] != "nil" and val2["type"] != "nil":
                sys.exit(ErrNum.INT_BAD_OPERANDS_TYPE)
            
            if (val1["type"] == "nil" and val2["type"] != "nil") or (val1["type"] != "nil" and val2["type"] == "nil"):
                edit_var(inst.args[0]["value"],"bool",False)
            else:
                comparison_output = retype_str_val(val1["type"],val1["value"]) == retype_str_val(val2["type"],val2["value"])
                edit_var(inst.args[0]["value"],"bool",comparison_output)

        elif inst.opcode == "AND":
            val1 = get_value_n_type(inst.args[1])
            val2 = get_value_n_type(inst.args[2])

            if val1["type"] != "bool" or val2["type"] != "bool":
                sys.exit(ErrNum.INT_BAD_OPERANDS_TYPE)
            
            operation_output = retype_str_val(val1["type"],val1["value"]) and retype_str_val(val2["type"],val2["value"])
            edit_var(inst.args[0]["value"],"bool",operation_output)
        
        elif inst.opcode == "OR":
            val1 = get_value_n_type(inst.args[1])
            val2 = get_value_n_type(inst.args[2])

            if val1["type"] != "bool" or val2["type"] != "bool":
                sys.exit(ErrNum.INT_BAD_OPERANDS_TYPE)
            
            operation_output = retype_str_val(val1["type"],val1["value"]) or retype_str_val(val2["type"],val2["value"])
            edit_var(inst.args[0]["value"],"bool",operation_output)

        elif inst.opcode == "NOT":
            val = get_value_n_type(inst.args[1])

            if val["type"] != "bool":
                sys.exit(ErrNum.INT_BAD_OPERANDS_TYPE)
            
            operation_output = not retype_str_val(val["type"],val["value"])
            edit_var(inst.args[0]["value"],"bool",operation_output)

        elif inst.opcode == "INT2CHAR":
            val = get_value_n_type(inst.args[1])

            if val["type"] != "int":
                sys.exit(ErrNum.INT_BAD_OPERANDS_TYPE)
            
            try:
                unicode_char = chr(int(val["value"]))
            except ValueError:
                sys.exit(ErrNum.INT_STRING_ERR)
            
            edit_var(inst.args[0]["value"],"string",unicode_char)
        
        elif inst.opcode == "STRI2INT":
            index = get_value_n_type(inst.args[2])
            text_str = get_value_n_type(inst.args[1])

            if text_str["type"] != "string" or index["type"] != "int" or inst.args[0]["type"] != "var":
                sys.exit(ErrNum.INT_BAD_OPERANDS_TYPE)
            
            if(int(index["value"]) < 0):
                sys.exit(ErrNum.INT_STRING_ERR)
            try:
                char = text_str["value"][int(index["value"])]
            except IndexError:
                sys.exit(ErrNum.INT_STRING_ERR)
            
            edit_var(inst.args[0]["value"],"int",ord(char))
        
        elif inst.opcode == "READ":
            try:
                in_data = arg_data["input"][read_index].rstrip()
            except IndexError:
                edit_var(inst.args[0]["value"],"nil",retype_str_val("nil","nil"))
                i += 1
                continue

            read_val = {"type":"nil","value":"nil"}
            if inst.args[1]["value"] == "nil":
                sys.exit(ErrNum.INT_BAD_OPERANDS_TYPE)
            
            if inst.args[1]["value"] == "bool":
                read_val["type"] = "bool"
                if in_data.lower() == "true":
                    read_val["value"] = True
                else:
                    read_val["value"] = False
            elif inst.args[1]["value"] == "string":
                read_val["type"] = "string"
                read_val["value"] = in_data
            elif inst.args[1]["value"] == "int":
                try:
                    in_data = int(in_data)
                except ValueError:
                    read_val["type"] = "nil"
                    read_val["value"] = Nil()
                else:
                    read_val["type"] = "int"
                    read_val["value"] = in_data
            
            edit_var(inst.args[0]["value"],read_val["type"],read_val["value"])
            read_index += 1
        
        elif inst.opcode == "WRITE":
            val = get_value_n_type(inst.args[0])
            if val["type"] == "bool":
                if val["value"] == True:
                    val["value"] = "true"
                else:
                    val["value"] = "false"
            if val["type"] == 'nil':
                val["value"] = ''

            print(val["value"],end="")
        
        elif inst.opcode == "CONCAT":
            val1 = get_value_n_type(inst.args[1])
            val2 = get_value_n_type(inst.args[2])

            if val1["type"] != "string" or val2["type"] != "string":
                sys.exit(ErrNum.INT_BAD_OPERANDS_TYPE)
            
            edit_var(inst.args[0]["value"],"string",val1["value"]+val2["value"])
        
        elif inst.opcode == "STRLEN":
            val = get_value_n_type(inst.args[1])

            if val["type"] != "string":
                sys.exit(ErrNum.INT_BAD_OPERANDS_TYPE)


            edit_var(inst.args[0]["value"],"int",len(val["value"]))
        
        elif inst.opcode == "GETCHAR":
            text_str = get_value_n_type(inst.args[1])
            index = get_value_n_type(inst.args[2])

            if text_str["type"] != "string" or index["type"]!= "int":
                sys.exit(ErrNum.INT_BAD_OPERANDS_TYPE)

            if(int(index["value"]) < 0):
                sys.exit(ErrNum.INT_STRING_ERR)
            
            try:
                char = text_str["value"][int(index["value"])]
            except IndexError:
                sys.exit(ErrNum.INT_STRING_ERR)
            
            edit_var(inst.args[0]["value"],"string",char)
        
        elif inst.opcode == 'SETCHAR':
            char = get_value_n_type(inst.args[2])
            index = get_value_n_type(inst.args[1])
            var = get_value_n_type(inst.args[0])

            if var["type"] != "string" or index["type"] != "int" or char["type"] != "string":
                sys.exit(ErrNum.INT_BAD_OPERANDS_TYPE)

            if(int(index["value"]) < 0):
                sys.exit(ErrNum.INT_STRING_ERR)

            try:
                var["value"] = list(var["value"])
                var["value"][int(index["value"])] = char["value"][0]
                var["value"] = "".join(var["value"])
            except IndexError:
                sys.exit(ErrNum.INT_STRING_ERR)

            edit_var(inst.args[0]["value"],"string",var["value"])
        
        elif inst.opcode == "TYPE":
            val_type = get_type_from_arg(inst.args[1])
            ret_type = ""
            
            if val_type is not None:
                ret_type = val_type
            
            edit_var(inst.args[0]["value"],"string",ret_type)
            
        elif inst.opcode == "JUMP":
            label = get_value_n_type(inst.args[0])
            i = find_label_index(label["value"],labels) + 1
            continue
        
        elif inst.opcode == "JUMPIFEQ":
            label = get_value_n_type(inst.args[0])
            val1 = get_value_n_type(inst.args[1])
            val2 = get_value_n_type(inst.args[2])
            label_index = find_label_index(label["value"],labels) + 1
            

            if val1["type"] != val2["type"] and val1["type"] != "nil" and val2["type"] != "nil":
                sys.exit(ErrNum.INT_BAD_OPERANDS_TYPE)
            
            if (val1["type"] == "nil" and val2["type"] != "nil") or (val1["type"] != "nil" and val2["type"] == "nil"):
                i += 1
                continue

            if val1["value"] == val2["value"]:
                i = label_index
                continue
            
        elif inst.opcode == "JUMPIFNEQ":
            label = get_value_n_type(inst.args[0])
            val1 = get_value_n_type(inst.args[1])
            val2 = get_value_n_type(inst.args[2])
            label_index = find_label_index(label["value"],labels) + 1

            if val1["type"] != val2["type"] and val1["type"] != "nil" and val2["type"] != "nil":
                sys.exit(ErrNum.INT_BAD_OPERANDS_TYPE)
            
            if (val1["type"] == "nil" and val2["type"] != "nil") or (val1["type"] != "nil" and val2["type"] == "nil"):
               i = label_index
               continue
            
            if val1["value"] != val2["value"]:
                i = label_index
                continue
        
        elif inst.opcode == "EXIT":
            val = get_value_n_type(inst.args[0])

            if val["type"] != "int":
                sys.exit(ErrNum.INT_BAD_OPERANDS_TYPE)
            if val["value"] < 0 or val["value"] > 49:
                sys.exit(ErrNum.INT_BAD_OPERAND_VALUE)
            
            sys.exit(val["value"])
        
        elif inst.opcode == "LABEL":
            i += 1
            continue

        elif inst.opcode == "DPRINT":
            val = get_value_n_type(inst.args[0])
        
        elif inst.opcode == "BREAK":
            print("Code position:",i,file=sys.stderr)
            print("GF:",i,file=sys.stderr,end=" ")
  
        i = i +1
    
sys.exit(0)
            
            


        
        

        

           
                

            
            
        





            
            





