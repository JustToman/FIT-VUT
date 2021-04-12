<?php
#Including used php files
include "scripts/scanner.php";
include "scripts/statistics.php";

/**
 * Determines whether line of tokens (Equals 1 Grammar rule) is allowed or not
 * 
 * @param array $tokens Tokens created out of one line of IPPcode21
 * @return bool True if rule is in grammar, False if rule isn't in grammar
 */
function is_in_grammar($tokens){
    return in_array($tokens,Grammar::GRAMMAR);
}


/**
 * Appends instruction to xml file
 * 
 * Creates new element for instruction and appends necessary element and attributes 
 * to generate correct xml file
 * 
 * @param DOMNode $xml_program Instance of DOMNode class for creating new elements and attributes
 * @param DOMElement $xml_appender Root element of $xml_program (Needed for creating it's child nodes)
 * @param array $tokens Array containing 2 items (also arrays) first item contains tokens and second
 * item contains strings containing source code values eg. [[ID,"GF@counter"]]
 * @param int $counter Counter of instructions (Needed for order attribute)
 */
function xml_append_instruction($xml_program,$xml_appender,$tokens,$counter){
    #Creating instruction element and it's attribute order
    $instruction_element = $xml_program->createElement('instruction');
    $instruction_att_order = $xml_program->createAttribute('order');
    $instruction_att_order->value = $counter;
    $instruction_element->appendChild($instruction_att_order);

    #Creating attribute opcode and appending it to instruction element
    $instruction_att_op_code = $xml_program->createAttribute('opcode');
    $instruction_att_op_code->value = strtoupper($tokens[1][0]);
    $instruction_element->appendChild($instruction_att_op_code);
   
    /*
    Looping through tokens and
    creating |arg(NUMBER)| [where (NUMBER) equals to arguments order 1st argument looks like:
    arg1] text element (it's value is literal), it's attribute |type| 
    and appending |type| to |argument| as it's child. Then appending |arg(NUMBER)| to
    |instruction| element as it's child

    Simultaneosly generating  needed types and literals with function get_types_and_literals($tokens)
    */
    foreach(array_slice($tokens[1],1) as $key => $value){
        $types_n_literals = get_types_and_literals($tokens);
        $instruction_elem_arg = $xml_program->createElement("arg".strval($key + 1),htmlspecialchars($types_n_literals[1][$key]));
        $instruction_element->appendChild($instruction_elem_arg);
        $arg_attr = $xml_program->createAttribute("type");
        $arg_attr->value = $types_n_literals[0][$key];
        $instruction_elem_arg->appendChild($arg_attr);
    }
    #Appending |instruction| element as $xml_appender child
    $xml_appender->appendChild($instruction_element);

}

/**
 * Gets types of tokens and it's literals
 * 
 * Gets types of tokens and it's literals and returns array containing 2 arrays of which
 * first contains types of tokens (var,bool,string,label) and second contains literal (string value,var name
 * ,etc).
 * @param array $tokens Array containing 2 items (also arrays) first item contains tokens and second
 * item contains strings containing source code values eg. [[ID,"GF@counter"]]
 * @return array array containing 2 arrays of which
 * first contains types of tokens (var,bool,string,label) and second contains literal (string value,var name
 * ,etc).
 */
function get_types_and_literals($tokens){
    $types = array();
    $literals = array();
    for($i = 1; $i < count($tokens[0]); $i++){
        if($tokens[0][$i] == TokenConstants::CONSTANT){
            if(preg_match("/^bool@(true|false)\z/u",$tokens[1][$i])){
                $types[$i - 1] = "bool";
                $literals[$i - 1] = preg_replace("/^bool@/","",$tokens[1][$i]);
            }
            elseif(preg_match("/^int@(\\+|-)?[0-9]+\z/u",$tokens[1][$i])){
                $types[$i - 1] = "int";
                $literals[$i - 1] = preg_replace("/^int@/","",$tokens[1][$i]);
            }
            elseif(preg_match("/^nil@nil\z/u",$tokens[1][$i])){
                $types[$i - 1] = "nil";
                $literals[$i - 1] = "nil";
            }
            else{
                $types[$i - 1] = "string";
                $literals[$i - 1] = preg_replace("/^string@/","",$tokens[1][$i]);

            }
        }
        elseif($tokens[0][$i] == TokenConstants::LABEL){
            $types[$i - 1] = "label";
            $literals[$i - 1] = $tokens[1][$i];
        }
        elseif($tokens[0][$i] == TokenConstants::ID){
            $types[$i - 1] = "var";
            $literals[$i - 1] = $tokens[1][$i];
        }
        elseif($tokens[0][$i] == TokenConstants::TYPE){
            $types[$i - 1] = "type";
            $literals[$i - 1] = $tokens[1][$i];
        }
    }

    return array($types,$literals);
}


/**
 * Parses input arguments along with format check, returns permissions class instance or exits with exit code
 * @param array $argv Array containing arguments on input
 * @param array $argc Number of arguments
 * @return StatisticsPermissionns[] containing info about statp extensions parameters
 */
function parse_args($argv,$argc){
   
    //Checking if help argument is present, if so, prints help
    if($argc == 2 && $argv[1] == "--help"){
        echo "Nápověda pro skript v jazyce php pro provedení lexikální a syntaktické analýzy jazyka .IPPCode21\n";
        echo "Verze php: 7.4.14\n";
        echo "Spuštění skriptu: php parser.php <název souboru>\n";
        echo "Výstup skriptu je xml soubor \"out.xml\"\n";
        echo "Zobrazení nápovědy: php parser.php --help\n";
        exit(0);
    }
    //If argc > 1 (1st arg is program name, implicitly), searching for STATP extension parameters
    //Along with checking for arguments, checking formats and arguments order
    elseif($argc > 1){
        //Number of --stat parameters (must be at least 0)
        $n_of_stat = -1; 
        //Array of permissions, it's size will be equal to number of --stat output files
        $permissions_array = array();
        //Allowed parameters (excluding --statp which is checked separately)
        $allowed_params = array("--loc","--comments","--labels","--jumps","--fwjumps","--backjumps",
                                "--badjumps");
       
        //Used_filenames needs to be stored for latter repetition check. (There can't be 2
        // stat argument with same file name )                        
        $used_filenames = array();
        for($i = 1; $i < $argc; $i = $i + 1)
        {
            //If --stat parameter with correct file name is present
            if(preg_match("/\A--stats=.*\z/",$argv[$i])){
                $n_of_stat += 1;  //increase number of --stat parameter
                $file_name = substr($argv[$i],8); //extract filename from parameter
               
                //If filename is used return File output error
                if (in_array($file_name,$used_filenames)){
                    exit(ErrNums::FILE_OUTPUT_ERROR);
                }

                //If not open file and include it's name in used_filename array
                $file = fopen($file_name,'w');
                $used_filenames[$n_of_stat] = $file_name;
                
                //If file opening wasn't successful return File output error
                if(!$file){
                    exit(ErrNums::FILE_OUTPUT_ERROR);
                }

                //Adding new Statistic permission to permission array (more in scripts/statistics.php)
                //Saving output file to permission array
                //Defining order array, which defines order of parameters (needed for correct output)
                $permissions_array[$n_of_stat] = new StatisticsPermissions();
                $permissions_array[$n_of_stat]->file = $file;
                $permissions_array[$n_of_stat]->order = array();
            }
            /*If argument isn't --stat, then it must be one of allowed_parameters
             If it is,argument is pushed to order array. If not, returns MISSING_PARAMETERS err */
            elseif (in_array($argv[$i],$allowed_params) && $n_of_stat >= 0){
                array_push($permissions_array[$n_of_stat]->order,$argv[$i]);
            }
            else{
                exit(ErrNums::MISSING_PARAMETERS);
            }
        }
        return $permissions_array;
    }
}

//Creating permissions array containing information about STATP extension's arguments
$permissions_array = parse_args($argv,$argc);

//Creating statistic instance
$statistics = new Statistics($permissions_array);

//Creating auxiliary arrays to store visited_labels (for backjumps) and possible_fw_jumps
//for saving labels, that wasn't created yet (or won't even be)
$visited_labels = array();
$possible_fw_jumps = array();



#opening stdin
$fr_stdin = NULL;
if(!($fr_stdin = fopen("php://stdin","r"))){
    exit(ErrNums::FILE_INPUT_ERROR);
}


#Loading first line of source code in form of tokens to $tokens array
$tokens = get_tokens($fr_stdin,$statistics);

#1st line of source code, should be header => 
#checking for header if it isn't present exits with Error Number
if($tokens[0] != Grammar::GRAMMAR[0]){
    exit(ErrNums::MISSING_HEADER);
}

#Loading new line of tokens and checking for some kind of inner error that could ocurr during lexical analysis
if(($tokens = get_tokens($fr_stdin,$statistics)) == array(ErrNums::INNER_ERROR)){
    exit(ErrNums::INNER_ERROR);
}

#Creating and setting up xml document and it's first elements
$xml = new DOMDocument('1.0','UTF-8');
$root_element = $xml->createElement("program");
$root_attribute = $xml->createAttribute("language");
$root_attribute->value = 'IPPcode21';
$root_element->appendChild($root_attribute);
$xml->appendChild($root_element); 



#While cycle that loads lines of code (in tokens)
#and checks whether they are in Grammar rules
$counter = 1;
while($tokens != array(TokenConstants::EOF))
{
    
    #Checking whether rule exist for given tokens
    if(!is_in_grammar($tokens[0])){
        #Checking for Bad Operation code error. If first token isn't keyword, then it exits with given Error Number
        if($tokens[0][0] == TokenConstants::ID || $tokens[0][0] == TokenConstants::TYPE || 
          $tokens[0][0] == TokenConstants::LABEL || $tokens[0][0] == TokenConstants::CONSTANT ||
          $tokens[0][0] == TokenConstants::LEX_ERR)
        {
            exit(ErrNums::BAD_OP_CODE);
        }
        
        exit(ErrNums::OTHERS);
    }

    #If header ocurrs more than once, return ERROR (BAD_OP_CODE)
    if($tokens[0] == Grammar::GRAMMAR[0]){
        exit(ErrNums::BAD_OP_CODE);
    }

    #Checking for label -> statistics
    if($tokens[0][0] == TokenConstants::KW_LABEL){
        array_push($visited_labels,$tokens[1][1]);
        $statistics->incNumOfLabels();
    }

    #Checking for jumps -> statistics
    if($tokens[0][0] == TokenConstants::KW_JUMP || $tokens[0][0] == TokenConstants::KW_JUMPIFEQ ||
       $tokens[0][0] == TokenConstants::KW_JUMPIFNEQ || $tokens[0][0] == TokenConstants::KW_CALL ||
       $tokens[0][0] == TokenConstants::KW_RETURN){

        $statistics->incNumOfJumps();
        if($tokens[0][0] != TokenConstants::KW_RETURN){
            if(in_array($tokens[1][1],$visited_labels)){
                $statistics->incNumOfBackJumps();
            }
            else{
                array_push($possible_fw_jumps,$tokens[1][1]);
            }
        }
    }
    
    xml_append_instruction($xml,$root_element,$tokens,$counter);


    #Loading new line of tokens and simultaniously checking for some INNER ERROR.
    if(($tokens = get_tokens($fr_stdin,$statistics)) == array(ErrNums::INNER_ERROR)){
        exit(ErrNums::INNER_ERROR);
    }
   

    $counter += 1;
}

$statistics->incNumberOfLines($counter - 1);

//Comparing values from $possible_fw_jumps vith visited_labels to determine
//whether label was created or wasn't and adding it to statistics
foreach($possible_fw_jumps as $value){
    if(in_array($value,$visited_labels)){
        $statistics->incNumOfForwardJumps();
    }
    else{
        $statistics->incNumOfBadJumps();
    }
}

//Writing statistics to file, if error occurs, inner error is returned
if(!empty($permissions_array)){
    if(!$statistics->writeStatistic()){
        exit(ErrNums::INNER_ERROR);
    }
}

# Formats xml, saves it and exits with code 0 (Source code passed)
$xml->formatOutput = true;
$xml_out = $xml->saveXML();
echo $xml_out;
exit(0);

?>