import argparse
import re
import sys
from pathlib import Path
from modules.constants import ErrNum
from modules.instruction import Instruction
import xml.etree.ElementTree as ET
from pprint import pprint

#Editing ArgumentParser class from argparse lib to exit with code 10
#whenever error appears
class ArgumentParser(argparse.ArgumentParser):
    def error(self,message):
        sys.exit(10)



def arg_parse():
    """
    Parses input args, if incorrect args are passed, exits with code 10.
    
    Accepted args:
    --help               -> prints help
    --input=<filepath>   -> file containing user input
    --source=<filepath>  -> file containing source code

    --help cannot be combined with other arguments.

    One of --input or --source must be passed (can be both).

    If one of those parameters (--input,--source) is missing
    it is additionaly added through stdin.

    Returns:
        dict: dictionary with two keys ("input","source"), 
        dict["input"] contains list of lines containing user input. 
        dict["source"] contains string with source code (Ippcode21 in xml form)
    """  

    #Parse arguments with help of argparse
    parser = ArgumentParser(add_help = False,usage="")
    parser.add_argument('--input',type=Path) #path to file should have libpath.Path type
    parser.add_argument("--source",type=Path) #path to file should have libpath.Path type
    parser.add_argument("--help",action='store_true')
    args = parser.parse_args()

    out_dict = {"input":list(),"source":""}

    
    #If help == True, all other arguments have to be excluded
    if args.help == True and args.input is None and args.source is None:
        print("TBD")
        sys.exit(ErrNum.OK)

    #If --input and --source params are both present, but in bad formatting
    if args.input is not None and args.source is not None and len(sys.argv) != 3:
        sys.exit(ErrNum.BAD_PARAM)

    #If --help isn't present and input is and it exists:
    if args.help == False and args.input is not None and args.input.is_file():

        #Open input file 
        with args.input.open() as input_file:
            #read line by line and append it to out_dict["input"] array
            for line in input_file:
                out_dict["input"].append(line)

        #Is args.source isn't present it will be read from stdin
        if args.source is None:
            #Checking if --input <file> wasn't given (Only --input=<file> is needed)
            if len(sys.argv) != 2:
                sys.exit(ErrNum.BAD_PARAM)

            #Loops and tries to get input until EOFError exception occurs (ctrl+D is pressed)
            #If line is successfuly read, it's concateneted with current out_dict["source"]
            #contents
            while(1):
                try:
                    line = input()
                except EOFError:
                    break
                out_dict["source"] += line + "\n"
            return out_dict

    #If --help isn't present and --source is and it exists:
    if args.help == False and args.source is not None and args.source.is_file():
       
       #Open input file
        with args.source.open() as source_file:
            #Loop by lines and concatenate it with out_dict["source"] string
            for line in source_file:
                out_dict["source"] += line
      
        #if --input param isn't present it will be read from stdin
        if args.input is None:
            #Checking if --source <file> wasn't given (Only --source=<file> is needed)
            if len(sys.argv) != 2:
                sys.exit(ErrNum.BAD_PARAM)
            #Loops and tries to get input until EOFError exception occurs (ctrl+D is pressed)
            #If line is successfuly read, it's concateneted with current out_dict["source"]
            #contents
            while(1):
                try:
                    line = input()
                except EOFError:
                    break

                out_dict["input"].append(line.rstrip())
        return out_dict
            

    
    sys.exit(ErrNum.BAD_PARAM)
    

#XML str -> instructions functions


def check_xml_validity(xml_src:str):
    """Checks whether string containing xml has valid xml syntax
    if it doesn't exits with code 31

    Args:
        xml_src (str): string containing xml code
    """
    try:
        ET.fromstring(xml_src)
    except:
        sys.exit(ErrNum.XML_FORMAT_ERR)


def xml_to_inst(xml_src:str):
    """Transforms xml source code representation to instructions
    During transformations, xml validity is checked (right tags, attributes,values) etc.
    If something invalid is found, program exits with code 32. (XML_UNEXPECTED_STRUCT)

    Args:
        xml_src (str): string containing xml source code

    Returns:
        list: list of Instruction objects containing all source code instructions (not sorted)
    """

    program = ET.fromstring(xml_src)
    instructions = list()

    if program.tag != "program":
        sys.exit(ErrNum.XML_UNEXPECTED_STRUCT)
    
    try:
        if program.attrib["language"].upper() != "IPPCODE21":
            sys.exit(ErrNum.XML_UNEXPECTED_STRUCT)
    except:
        sys.exit(ErrNum.XML_UNEXPECTED_STRUCT)

    #loops through all instruction elements
    for instruction in program:
        args = list()

        #Check if element has right tag
        if instruction.tag != "instruction":
            sys.exit(ErrNum.XML_UNEXPECTED_STRUCT)
        
        #Check if element has order and opcode attribute
        if "order" not in instruction.attrib.keys() or "opcode" not in instruction.attrib.keys():
            sys.exit(ErrNum.XML_UNEXPECTED_STRUCT)

        #Check if order is integer value and casting it to integer
        try:
            order = instruction.attrib["order"]
            order = int(order)
        except:
            sys.exit(ErrNum.XML_UNEXPECTED_STRUCT)
            
        #Checking for instruction arguments
        for elem in instruction:
            arg = dict()
            #Matching if arguments have right format
            if re.match(r"^arg[1-9]+$",elem.tag) is None:
                sys.exit(ErrNum.XML_UNEXPECTED_STRUCT)

            #checking whether arguments has "type"  attribute
            try:
                arg["type"] = elem.attrib["type"]
            except:
                sys.exit(ErrNum.XML_UNEXPECTED_STRUCT)

            #replacing arg string with empty string (to get only order) and casting it to int
            arg["order"] = int(re.sub(r"arg","",elem.tag))   
            #Saving argument value
            arg["value"] = elem.text
            #Checking if arg["value"] is None (That means that argument has no value == empty_string )
            if(arg["value"] == None):
                arg["value"] = ""
            args.append(arg)
        #opcode to upper because instructions opcodes are case insensitive
        instructions.append(Instruction(order,instruction.attrib["opcode"].upper(),args))
    return instructions


def sort_instructions(instructions:list):
    """Sorts instructions by order attribute and
    checks whether they don't have order < 1 or duplicate 
    order. If they have, exits with code 32 (XML_UNEXPECTED_STRUCT)
    
    Args:
        instructions (list): List of Instruction instances
    
    Returns:
        list: list of instructions sorted by order
    """

    sorted_inst = sorted(instructions,key=lambda instruction: instruction.order)
    #Checks for order < 1
    if len(sorted_inst) > 0 and sorted_inst[0].order < 1:
        sys.exit(ErrNum.XML_UNEXPECTED_STRUCT)
    
    #Checks for duplicate order
    for i in range(1,len(sorted_inst)):
        if sorted_inst[i-1].order == sorted_inst[i].order:
            sys.exit(ErrNum.XML_UNEXPECTED_STRUCT)
    return instructions

#Instruction arguments functions 

def order_sort_key(arg):
    """Just auxilary function for sorting
        
    Returns:
        int: order of argument
    """
    return arg["order"]

def sort_args(instructions:list):
    """Sorts arguments of instructions by order

    Args:
        instructions (list): List of all Instruction instances defining src code
    
    Returns:
        list: list of instructions with arguments sorted by order
    """

    for i in range(len(instructions)):
        instructions[i].args = sorted(instructions[i].args,key = order_sort_key)
    return instructions

def check_args_orders(instructions:list):
    """ Checks whether instructions arguments doesn't have duplicate order. If they
    do, exits with code 32 (XML_UNEXPECTED_STRUCT)

    Args:
        instructions (list): List of all Instruction instances defining src code
    """
    for inst in instructions:
        for arg_index in range(len(inst.args)):
            if inst.args[arg_index]["order"] != arg_index + 1:
                sys.exit(ErrNum.XML_UNEXPECTED_STRUCT)





            
    




        

  
