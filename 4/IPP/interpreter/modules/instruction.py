class Instruction:
    """Represents one instruction of ippCode21 language

    Attributes:
        order     (int): order of instruction in code
        opcode (string): operation code of instruction
        args     (list): list of instruction arguments (Every element
        of list is dictionary containing info about order,value and type of argument)

    Methods:
        __init__(order,opcode,args)
    """
    def __init__(self,order,opcode,args):
        self.order = order
        self.opcode = opcode
        self.args = args
    