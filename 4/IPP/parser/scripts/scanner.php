<?php
#Module containing functions which perform or help perform lexical analysis
include "constants.php";


/**
 * Function used by parser to get next line of tokens
 * This function doesn't return just one token but array of tokens on one line
 * This function performs lexical analyser (Scanner) function. (Performs lexical analysis and transforms source code to tokens)
 * It returns one whole line of token because, then it could be easily compared with
 * Grammar rules.
 * 
 * @param file $file Source code to be scanned
 * @return array Returns array of tokens
 */
function get_tokens($file, &$statistics)
{
  #Variable for storing one line of source code
  $line = NULL;  
  #Variable for storing splitted line of source code
  $splitted_string = NULL;

  #Cycle which iterates through splitted string, performs lexical analysis and tokenize one line of source code
  #Removes all comments,redundant whitespaces (including trailing newline)
  while(1)
  {
    #Performs line load and checks for errors in loading line from file
    if(!($line = fgets($file))){
        #If error ocurred or EOF is reached, returns respectively return values
        if(feof($file)){
            #EOF token is returned in array just for return constistency
            return array(TokenConstants::EOF);
        }
        #INNER_ERROR Err Number is returned in array just for return constistency
        return array(ErrNums::INNER_ERROR);
    }

    if(strpos($line,"#") !== FALSE){
        $statistics->incNumOfComments();
    }

    #If line isnt blank space or comment
    if(!preg_match("/^#.*$/",$line) && !preg_match("/^[\s]$/",$line)){
    
        #Changes multiple whitespaces to one whitespace
        $line = preg_replace("/\s+/"," ",$line); 
        #Changes comments to blank space
        $line = preg_replace("/#.*/","",$line);
          
        #Trimms whitespaces from beginning and end of string
        $line = trim($line);

        #Splits string to array, where delimiter is space
        $splitted_string = explode(" ",$line);

        #Creating token array and assigning token values depending on preg_match() evaluation
        $token_arr = array();
        $values_arr = array();
        foreach($splitted_string as $key => $value){
            #Loading tokens to returning array
            $token_arr[$key] = which_token($value);
            $values_arr[$key] = $value;
            
            #Checking if token needs to be label and if it matches label regex
            if($key > 0 && needs_label($token_arr[$key-1]) && preg_match(TokenConstants::REGEX_ARR[TokenConstants::LABEL],$splitted_string[$key])){
                $token_arr[$key] = TokenConstants::LABEL;
            }

        }
        return array($token_arr,$values_arr);

    }
  }
}

/**
 * Iterates through array containing Regular expressions for matching Tokens (Class Tokens in constants.php)
 * and returns Number of token or LEX_ERR
 * 
 * @param string $str This function accepts string containing  string (one word) from source code
 * @return TokenConstants::CONSTANT defining which token matched regex
 */
function which_token($str)
{
    foreach(TokenConstants::REGEX_ARR as $key => $value)
    {
        if(preg_match($value,$str)){
            return $key;
        }
    }
    return TokenConstants::LEX_ERR;
}

/**
 * Determines whether token needs label after it or not. (Because of case, where keyword can be label).
 * This function is used to determine if token needs to be label or keyword (If it's name == to any keyword)
 * 
 * @param TokenConstants::CONSTANT $prev_token Token, which is check 
 * @return bool if token type is one of specified token types, that needs label, returns true. If not, returns false.
 */
function needs_label($prev_token)
{
    #If token == to one of those token types, then it needs to be label and fuction returns true
    return($prev_token == TokenConstants::KW_CALL ||$prev_token == TokenConstants::KW_LABEL ||$prev_token == TokenConstants::KW_JUMP
      ||$prev_token == TokenConstants::KW_JUMPIFEQ || $prev_token == TokenConstants::KW_JUMPIFNEQ);
}



?>