<?php
include "test_scripts/structures.php";

/**
 * Function that parses arguments for testing
 * @param int $argc number of arguments on input (0th argument is always name of program, so there will always be one)
 * @param array $argv array of string containing arguments
 * @return InputParameters structure containing info about parameters
 */
function parse_args($argc,$argv){

    $input_parameters = new InputParameters;
    $input_parameters->used_by_user = array();
    //Checks where arg help is combined with another arguments
    if($argc == 2 && $argv[1] == '--help'){
        echo "TBD\n";
        exit(0);
    }

    if($argc > 2 && $argv[1] != "--help"){
        $path_regex = "/.+=/";
    
        //Saving data to InputParameters structures, which will be returned
        for($key = 1; $key < count($argv); $key += 1){
            $parameter = preg_replace("/=.+/","",$argv[$key]);
            switch($parameter){
                case "--directory":
                    //Checking if parameter has correct form
                    if(!preg_match("/--directory=.+/",$argv[$key])){
                        exit(ErrNums::BAD_PARAMETERS);
                    }
                    $input_parameters->directory[1] = preg_replace($path_regex,"",$argv[$key]);
                    break;
                case "--recursive":
                    //Checking if parameter has correct form
                    if($argv[$key] != "--recursive"){
                        exit(ErrNums::BAD_PARAMETERS);
                    }
                    $input_parameters->recursive = TRUE;
                    break;
                case "--parse-script":
                    //Checking if parameter has correct form
                    if(!preg_match("/--parse-script=.+/",$argv[$key])){
                        exit(ErrNums::BAD_PARAMETERS);
                    }
                    //Setting parse script to true (TRUE means,that later it will be checked whether this file exists)
                    $input_parameters->parse_script[0] = TRUE;
                    //changes default parse script path to given path
                    $input_parameters->parse_script[1] = preg_replace($path_regex,"",$argv[$key]);
                    break;
                case "--int-script":
                    if(!preg_match("/--int-script=.+/",$argv[$key])){
                        exit(ErrNums::BAD_PARAMETERS);
                    }
                    //Setting int script to true (TRUE means that later it will be checke whether this file exists)
                    $input_parameters->int_script[0] = TRUE;
                    //changes default interpreter script path to given path
                    $input_parameters->int_script[1] = preg_replace($path_regex,"",$argv[$key]);
                    break;
                case "--parse-only":
                    if($argv[$key] != "--parse-only"){
                        exit(ErrNums::BAD_PARAMETERS);
                    }
                    /**
                     * Parse script, parse only and jexam files are set to true
                     * when parse-only is set.
                     */
                    $input_parameters->parse_script[0] = TRUE;
                    $input_parameters->parse_only = TRUE;
                    $input_parameters->jexamcfg[0] = TRUE;
                    $input_parameters->jexamxml[0] = TRUE;
                    break;
                case "--int-only":
                    if($argv[$key] != "--int-only"){
                        exit(ErrNums::BAD_PARAMETERS);
                    }
                    /**
                     * int script and int only are set to true.
                     */
                    $input_parameters->int_script[0]= TRUE;
                    $input_parameters->int_only = TRUE;
                    break;
                case "--jexamxml":
                    if(!preg_match("/--jexamxml=.+/",$argv[$key])){
                        exit(ErrNums::BAD_PARAMETERS);
                    }
                    $input_parameters->jexamxml[1] = preg_replace($path_regex,"",$argv[$key]);
                    break;
                case "--jexamcfg":
                    if(!preg_match("/--jexamcfg=.+/",$argv[$key])){
                        exit(ErrNums::BAD_PARAMETERS);
                    }
                    $input_parameters->jexamcfg[1] = preg_replace($path_regex,"",$argv[$key]);
                    break;
                default:
                    exit(ErrNums::BAD_PARAMETERS);
            }
            array_push($input_parameters->used_by_user,$parameter);
        }
    }
        //Checking for parameters colissions
        if(($input_parameters->parse_only && ($input_parameters->int_only || $input_parameters->int_script[0])) ||
            ($input_parameters->int_only && ($input_parameters->parse_only|| $input_parameters->parse_script[0]))){
                exit(ErrNums::BAD_PARAMETERS);
        }
    
        //parse-only == false and int-only == false
        //means that both interpreter and parser will be tested
        //This turns on both parse_only and int_only and allow to search if
        //their script paths exists
        if(!$input_parameters->parse_only && !$input_parameters->int_only){
            $input_parameters->parse_only = TRUE;
            $input_parameters->int_only = TRUE;
            $input_parameters->parse_script[0] = TRUE;
            $input_parameters->int_script[0] = TRUE;
        }
    
        //Checking for readability and writability of paths
        if(!is_executable($input_parameters->directory[1])){
            exit(ErrNums::PARAMETER_FILE_ERR);
        }
    
        if($input_parameters->parse_script[0] && !is_readable($input_parameters->parse_script[1])){
            exit(ErrNums::PARAMETER_FILE_ERR);
        }
     
       
        if($input_parameters->int_script[0] && !is_readable($input_parameters->int_script[1])){
            exit(ErrNums::PARAMETER_FILE_ERR);
        }

        if($input_parameters->jexamcfg[0] && !is_readable($input_parameters->jexamcfg[1])){
            exit(ErrNums::PARAMETER_FILE_ERR);
        }

        if($input_parameters->jexamxml[0] && !is_readable($input_parameters->jexamxml[1])){
            exit(ErrNums::PARAMETER_FILE_ERR);
        }

    return $input_parameters;
}

/**
 * Function that recursively
 * scans directory, finds .src and files that belongs to it .in, .out. ,.rc
 * If some of these belonging files doesn't exist, it will be created
 * @param string $path Path to scanned directory
 * @param boolean $is_recursive determines wheter directory should be scanned recursively
 * @return array $test_cases returns array of instances of TestCasePaths class
 */
function scan_dir($path, $is_recursive){
    $files = NULL; //Creating array containing all files
    $test_cases = array(); //returning array

    #if scandir on path fails, Parameter file error is raised
    if(!($files = scandir($path))){
        exit(ErrNums::PARAMETER_FILE_ERR);
    }

    //Loops through all 
    foreach($files as $file){
        //If file is directory and it's recursive and isn't relative path to this or prev dir
        //calls scan_dir on this file and merges $test_cases it with it's output
        if(is_dir($path."/".$file) && $is_recursive && ($file != '.' && $file != "..")){
            $test_cases = array_merge($test_cases,scan_dir($path."/".$file,TRUE));
        }
        //If it's not directory and it matches some_name.src regex
        //Paths to files are saved
        //If files doesn't exist they are created
        elseif(is_file($path."/".$file) && preg_match("/\A.+\.src\z/",$file)){
            //Crop just filename without extension
            $test_name = substr_replace($file,"",strlen($file)-4);
            //Create valid filepaths to other files needed for testing
            $in_path = $path."/".$test_name.".in";
            $out_path = $path."/".$test_name.".out";
            $rc_path = $path."/".$test_name.".rc";
            $src_path = $path."/".$file;
            //Check whether they exist, eventually create them
            if(!file_exists($in_path)){
                if(!touch($in_path)){
                    exit(ErrNums::FILE_IN_ERR);
                }
            }
            if(!file_exists($out_path)){
                if(!touch($out_path)){
                    exit(ErrNums::FILE_IN_ERR);
                }
            }
            if(!file_exists($rc_path)){
                $new_rc_file = fopen($rc_path,'w');
                if(!$new_rc_file){
                        exit(rrNums::FILE_IN_ERR);
                }
                if(!fwrite($new_rc_file,"0")){
                    exit(ErrNums::INNER_ERR);
                }
                if(!fclose($new_rc_file)){
                    exit(ErrNums::FILE_OUT_ERR);
                }
            }
            //Push test_case to $test_cases and set paths
            array_push($test_cases,new TestCasePaths);
            $test_cases[count($test_cases)-1]->src = $src_path;
            $test_cases[count($test_cases)-1]->name = $test_name;
            $test_cases[count($test_cases)-1]->in = $in_path;
            $test_cases[count($test_cases)-1]->out = $out_path;
            $test_cases[count($test_cases)-1]->rc = $rc_path;
             
        }
    }
    return $test_cases;
}


/**
 * Runs parser, then interpreter and then diff on .out file and interpreter output
 * It tests both parser and interpreter
 * @param InputParameters $input_parameters structure containing info about tools and scripts used in this test
 * @return list returns list of test case results ready to be transformed to html
 */
function test_parser_and_interpreter($input_parameters){
    //Scans directory for test cases
    $test_cases = scan_dir($input_parameters->directory[1],$input_parameters->recursive);
    //Creates array for return
    $test_case_results = array();

    //Loops through all test_cases
    foreach($test_cases as $key => $test_case){
        
        //Parser execution
        $parser_output;
        $parser_retval;
        $parser_command = "php ".$input_parameters->parse_script[1]." < ".$test_case->src;
        exec($parser_command,$parser_output,$parser_retval);

        //Making string from parser output array
        $parser_output = implode("\n",$parser_output);

        //Creating named tempfile to store parsers output to file
        $temp_out_path = tempnam(".","tmp");
        if(!$temp_out_path){
            exit(ErrNums::INNER_ERR);
        }

        $temp_out = fopen($temp_out_path,"w");

        if(!$temp_out){
            unlink($temp_out_path);
            exit(ErrNums::FILE_OUT_ERR);
        }

        //Writing parsers output to tempfile
        fwrite($temp_out,$parser_output);
        fclose($temp_out);


        //Interpret execution
        $interpret_output;
        $interpret_retval;

        $interpret_command = "python3 ".$input_parameters->int_script[1]." --source=".$temp_out_path." --input=".$test_case->in;

        exec($interpret_command,$interpret_output,$interpret_retval);
        
        //Making string from interpret output array
        $interpret_output = implode("\n",$interpret_output);

        //Writing interpret output to tempfile
        $temp_out = fopen($temp_out_path,"w");

        if(!$temp_out){
            unlink($temp_out_path);
            exit(ErrNums::FILE_OUT_ERR);
        }


        fwrite($temp_out,$interpret_output);

        fclose($temp_out);


        //Executing dif on ref output and real interpret output (diff == 0 => files are the same)
        $diff_retval;
        $diff_output;
        $diff_command = "diff ".$temp_out_path." ".$test_case->out;
        exec($diff_command,$diff_output,$diff_retval);
        

        //Unlink of tempfile!
        unlink($temp_out_path);
        

        //Opening and reading file containing ref. retval
        $rc_file = fopen($test_case->rc,"r");

        if(!$rc_file){
            exit(ErrNums::FILE_IN_ERR);
        }

        $rc_contents = fread($rc_file,filesize($test_case->rc));
        fclose($rc_file);

        //opening and reading file containing ref. output
        $out_file = fopen($test_case->out,"r");

       
        if(!$out_file){
            exit(ErrNums::FILE_IN_ERR);
        }

        //If output doesn't exist, save just empty string to out_contents
        $out_contents = (filesize($test_case->out) > 0)?fread($out_file,filesize($test_case->out)):"";

        //Save test results to returning array
        $test_case_results[$key] = new TestCaseResult;
        $test_case_results[$key]->ref_retval = $rc_contents;
        $test_case_results[$key]->real_retval = strval($interpret_retval);
        $test_case_results[$key]->is_ok = (intval($rc_contents) == $interpret_retval && $diff_retval == 0);
        $test_case_results[$key]->name = $test_case->name;
        $test_case_results[$key]->real_output = explode("\n",$interpret_output);
        $test_case_results[$key]->ref_output = explode("\n",$out_contents);
        $test_case_results[$key]->path = dirname($test_case->src).DIRECTORY_SEPARATOR;
        fclose($out_file);  
    }
    return $test_case_results;
        
}

/**
 * Runs parser, then jexamxml to compare xml
 * It tests only parser
 * @param InputParameters $input_parameters structure containing info about tools and scripts used in this test
 * @return list returns list of test case results ready to be transformed to html
 */
function test_parser($input_parameters){
    //Scans directory for test cases
    $test_cases = scan_dir($input_parameters->directory[1],$input_parameters->recursive);
    //Create array for return
    $test_case_results = array();

    //Loops through all test cases
    foreach($test_cases as $key => $test_case){
        
        //Parser execution
        $parser_output;
        $parser_retval;
        $parser_command = "php 7.4".$input_parameters->parse_script[1]." < ".$test_case->src;
        exec($parser_command,$parser_output,$parser_retval);

        //Making string from parser output array
        $parser_output = implode("\n",$parser_output);

        //Creating named tempfile to store parsers output to file
        $temp_out_path = tempnam(".","tmp");
        if(!$temp_out_path){
            exit(ErrNums::INNER_ERR);
        }
        
        $temp_out = fopen($temp_out_path,"w");
        
        if(!$temp_out){
            unlink($temp_out_path);
            exit(ErrNums::FILE_OUT_ERR);
        }

        fwrite($temp_out,$parser_output);
       
        fclose($temp_out);
        
        //Jexaxml init
        $jexamxml_command = "java -jar ".$input_parameters->jexamxml[1]." ".$test_case->out." ".$temp_out_path." ".$temp_out_path." ".$input_parameters->jexamcfg[1]." 2>/dev/null";
        $jexamxml_retval;
        $jexamxml_output;

        //saving whether both files are empty
        $are_empty = !(filesize($test_case->out) + filesize($test_case->out));

        //if files aren't empty and jexamxml execution fails, return INNER ERROR
        //Jexaxml works wrong with empty files, this guarantees, that jexamxml won't be
        //executed if files are empty (because of short-circuit evaluation)
        if(!$are_empty && !exec($jexamxml_command,$jexamxml_output,$jexamxml_retval)){
            exit(ErrNums::INNER_ERR);
        }
        unlink($temp_out_path);
        
        //If files are empty retval is 0 (they are the same), if not
        //retval is taken from jexamxml execution
        $jexamxml_retval = ($are_empty)?0:$jexamxml_retval;
        
        //Opening and reading ref. retval file
        $rc_file = fopen($test_case->rc,"r");

        if(!$rc_file){
            exit(ErrNums::FILE_IN_ERR);
        }

        $rc_contents = fread($rc_file,filesize($test_case->rc));

        fclose($rc_file);


        //Opening and reading ref. output file
        $out_file = fopen($test_case->out,"r");

        if(!$out_file){
            exit(ErrNums::FILE_IN_ERR);
        }
        
        //If ref. output file is empty, saves just empty string. If not, saves whole file
        $out_contents = (filesize($test_case->out) > 0)?fread($out_file,filesize($test_case->out)):"";

        fclose($out_file);

        //Sets test case results
        $test_case_results[$key] = new TestCaseResult;
        $test_case_results[$key]->ref_retval = $rc_contents;
        $test_case_results[$key]->real_retval = strval($parser_retval);
        //if ref and real retvals are the same and jexamxml_retval == 0 -> test passed
        $test_case_results[$key]->is_ok = (($rc_contents == strval($parser_retval)) && (!$jexamxml_retval)); 
        $test_case_results[$key]->name = $test_case->name;
        $test_case_results[$key]->real_output = explode("\n",$parser_output);
        $test_case_results[$key]->ref_output = explode("\n",$out_contents);
        $test_case_results[$key]->path = dirname($test_case->src).DIRECTORY_SEPARATOR;
       
    }
    return $test_case_results;
}

/**
 * Runs interpreter, then diff to compare real output with ref output
 * It tests only interpreter
 * @param InputParameters $input_parameters structure containing info about tools and scripts used in this test
 * @return list returns list of test case results ready to be transformed to html
 */
function test_interpreter($input_parameters){
    //Scans directory for test cases
    $test_cases = scan_dir($input_parameters->directory[1],$input_parameters->recursive);
    //Create array for return
    $test_case_results = array();

    //Loops through all test cases
    foreach($test_cases as $key => $test_case){
        
         //Interpreter execution
        $interpret_output;
        $interpret_retval;
        $interpret_command = "python3 ".$input_parameters->int_script[1]." --source=".$test_case->src." --input=".$test_case->in;
        exec($interpret_command,$interpret_output,$interpret_retval);
        
        //Making string from interpreter output array
        $interpret_output = implode("\n",$interpret_output);

        //Creating named tempfile to store interprets output to file
        //Just tempfile creation,opening and writing
        $temp_out_path = tempnam(".","tmp");
        if(!$temp_out_path){
            exit(ErrNums::INNER_ERR);
        }

        $temp_out = fopen($temp_out_path,"w");

        if(!$temp_out){
            unlink($temp_out_path);
            exit(ErrNums::FILE_OUT_ERR);
        }

        fwrite($temp_out,$interpret_output);

        fclose($temp_out);
        

        //Exectuion of diff (diff == 0 => files are the same)
        $diff_retval;
        $diff_output;
        $diff_command = "diff ".$temp_out_path." ".$test_case->out;
        exec($diff_command,$diff_output,$diff_retval);

        
        //Unlinking temp file
        unlink($temp_out_path);
        

        //Opening,reading and closing file containing interpreter ref. retval
        $rc_file = fopen($test_case->rc,"r");

        if(!$rc_file){
            exit(ErrNums::FILE_IN_ERR);
        }

        $rc_contents = fread($rc_file,filesize($test_case->rc));
        fclose($rc_file);

        //Opening, readning and closing file containing interpreter ref. output
        $out_file = fopen($test_case->out,"r");

       
        if(!$out_file){
            exit(ErrNums::FILE_IN_ERR);
        }

        //if out_file is empty save just empty string, if not, save all file contents
        $out_contents = (filesize($test_case->out) > 0)?fread($out_file,filesize($test_case->out)):"";

        $test_case_results[$key] = new TestCaseResult;
        $test_case_results[$key]->ref_retval = $rc_contents;
        $test_case_results[$key]->real_retval = strval($interpret_retval);
        //if ref and real retval equals and diff_retval == 0 => test passed
        $test_case_results[$key]->is_ok = (intval($rc_contents) == $interpret_retval && $diff_retval == 0);
        $test_case_results[$key]->name = $test_case->name;
        $test_case_results[$key]->real_output = explode("\n",$interpret_output);
        $test_case_results[$key]->ref_output = explode("\n",$out_contents);
        $test_case_results[$key]->path = dirname($test_case->src).DIRECTORY_SEPARATOR;
        fclose($out_file);  
    }
    return $test_case_results;
}

/**
 * Generates html output containing test case info
 * @param list $test containing info about test case (name,result,outputs,retvals,etc..)
 * @param list $input_parameters containing info about parameters
 * @return String containing html with test results
 */
function generate_html($tests,$input_parameters){
    $n_of_ok_tests = count_successful_tests($tests); 

    //Creating new doc
    $dom = new DOMDocument('1.0');
    
    //Creating page styling
    $style_body = "\nbody {\nbackground-color: rgb(45,43,43);\ncolor:white;\n}\n";
    $style_h1 = "h1 {\ncolor:rgb(255,255,255);\ntext-align:center;\n}\n";
    $style_th = "th {\npadding-right:5px;\nfont-size:xx-large;\ntext-align:left;\n}\n";
    $style_td = "td {\nfont-size: xx-large;\n}\n";
    $style_outter_div = ".outter_div {\nfont-size: x-large;\nmargin: 10px\npadding:10px;\nborder:8px groove black;\n}\n";
    $style_inner_div = ".inner_div {\nmargin: 0;\ntext-align: center;\nfont-size:xx-large;\n}\n";
    $style_i = "i {\ncolor:#00FFFE;\n}\n";


    //html tag creation
    $html = $dom->createElement("html");
    $dom->appendChild($html);

    //head tag creating
    $head = $dom->createElement("head");
    $html->appendChild($head);

    //Adding styles
    $style = $dom->createElement("style");
    $head->appendChild($style);

    $style->nodeValue = $style_body.$style_h1.$style_th.$style_td.$style_outter_div.$style_inner_div.$style_i;

    //Adding body
    $body = $dom->createElement("body");
    $html->appendChild($body);

    $h1 = $dom->createElement("h1","Výsledky testů IPP 2021");
    $body->appendChild($h1);


    $table = $dom->createElement("table");
    $body->appendChild($table);

    //Creating table containing test results info
    $table_contents = array();
    $table_contents[0] = array("Počet testů:",strval(count($tests)),"color:white");
    $table_contents[1] = array("Úspěšné testy:",strval($n_of_ok_tests),"color:greenyellow");
    $table_contents[2] = array("Neúspěšné testy:",strval(count($tests)-$n_of_ok_tests),"color:red");
    $table_contents[3] = array("Použité argumenty:",implode(" ",$input_parameters->used_by_user),"color:rgb(162,162,162)");
    $table_contents[4] = array("Procentuální úspěšnost:",strval(round(($n_of_ok_tests * 100)/((count($tests) > 0)?count($tests):$n_of_ok_tests)))."%","color:white");

    //Adding table to doc
    foreach($table_contents as $content){
        $tr = $dom->createElement("tr");
        $table->appendChild($tr);
    
        $th = $dom->createElement("th",$content[0]);
        $tr->appendChild($th);
    
        $td = $dom->createElement("td",$content[1]);
        $tr->appendChild($td);
    
        $style_attr = $dom->createAttribute("style");
        $style_attr->value = $content[2];
        $td->appendChild($style_attr);
    }

    $h1 = $dom->createElement("h1","Rozpis testů");
    $body->appendChild($h1);

    //Adding all tests to doc
    foreach($tests as $key => $test){
        $inner_text = array("Úspěch","color:greenyellow");
        if(!$test->is_ok){
            $inner_text = array("Neúspěch","color:red");
        }
        
        $outter_div = $dom->createElement("div");
        $body->appendChild($outter_div);
        $class_attribute = $dom->createAttribute("class");
        $class_attribute->value = "outter_div";
        $outter_div->appendChild($class_attribute);
        $inner_div = $dom->createElement("div");
        $class_attribute = $dom->createAttribute("class");
        $class_attribute->value = "inner_div";
        $inner_div->appendChild($class_attribute);
        $span = $dom->createElement("span",$inner_text[0]);
        $span_att = $dom->createAttribute("style");
        $span_att->value = $inner_text[1];
        $inner_div->appendChild($span);
        $span->appendChild($span_att);
        $outter_div->appendChild($inner_div);
      
        $text = $dom->createTextNode("Číslo testu: ");
        $bold = $dom->createElement("b", strval($key + 1));
        $br = $dom->createElement("br");
        $outter_div->appendChild($text);
        $outter_div->appendChild($bold);
        $outter_div->appendChild($br);

        $text = $dom->createTextNode("TestCase: ");
        $bold = $dom->createElement("b",$test->name);
        $br = $dom->createElement("br");
        $outter_div->appendChild($text);
        $outter_div->appendChild($bold);
        $outter_div->appendChild($br);

        $text = $dom->createTextNode("Cesta k testu: ");
        $bold = $dom->createElement("b", $test->path);
        $br = $dom->createElement("br");
        $outter_div->appendChild($text);
        $outter_div->appendChild($bold);
        $outter_div->appendChild($br);

        //If test is ok print just one return code (they are both the same)
        if($test->is_ok){
            $text = $dom->createTextNode("Návratový kód: ");
            $bold = $dom->createElement("b", $test->real_retval);
            $br = $dom->createElement("br");
            $outter_div->appendChild($text);
            $outter_div->appendChild($bold);
            $outter_div->appendChild($br);
        }
        //If test failed print both return codes (reference and real)
        else{
            $text = $dom->createTextNode("Referenční návratový kód: ");
            $bold = $dom->createElement("b", $test->ref_retval);
            $br = $dom->createElement("br");
            $outter_div->appendChild($text);
            $outter_div->appendChild($bold);
            $outter_div->appendChild($br);

            $text = $dom->createTextNode("Váš návratový kód: ");
            $bold = $dom->createElement("b", $test->real_retval);
            $br = $dom->createElement("br");
            $outter_div->appendChild($text);
            $outter_div->appendChild($bold);
            $outter_div->appendChild($br);

            //Printing of real and ref output
            if($test->real_retval == 0 || $input_parameters->int_only){
                $text = $dom->createTextNode("Váš výstup: ");
                $outter_div->appendChild($text);
                $br = $dom->createElement("br");
                $outter_div->appendChild($br);
                foreach($test->real_output as $line){
                    $italic = $dom->createElement("i", $line);
                    $br = $dom->createElement("br");
                    $outter_div->appendChild($italic);
                    $outter_div->appendChild($br);
                }
            }
            if($test->ref_retval == 0 || $input_parameters->int_only){
                $text = $dom->createTextNode("Referenční výstup: ");
                $outter_div->appendChild($text);
                $br = $dom->createElement("br");
                $outter_div->appendChild($br);
                foreach($test->ref_output as $line){
                    $italic = $dom->createElement("i", $line);
                    $br = $dom->createElement("br");
                    $outter_div->appendChild($italic);
                    $outter_div->appendChild($br);
                }
            }

        }
    }

    return "<!DOCTYPE html>\n".$dom->saveHTML();
}

/**
 * html doc formatting of created document and it's printing to stdout
 * @param string $doc generated html doc
 */
function fix_html_format($doc){
    $dom = new DOMDocument();
    $dom->preserveWhiteSpace = false;
    $dom->loadHTML($doc);
    $dom->formatOutput = true;
    echo $dom->saveHTML();
}

/**
 * Counts number of successful tests
 * @param array $results array of instances of TestCaseResult class
 * @return int number of successful tests
 */
function count_successful_tests($results){
    $counter = 0;
    foreach($results as $value){
        if($value->is_ok){
            $counter += 1;
        }
    }
    return $counter;
}

$parameters = parse_args($argc,$argv);
$tests;
if(!$parameters->int_only){
    $tests = test_parser($parameters);
}
if(!$parameters->parse_only){
    $tests = test_interpreter($parameters);
}
else{
    $tests = test_parser_and_interpreter($parameters);
}
fix_html_format(generate_html($tests,$parameters));

exit(ErrNums::OK);



?>