<?php
    /**
     * Structure-like class containing info about Input parameters
     * 
     * Parameters containing path to file are arrays, 
     * where 0th element determines wheter argument is used
     * 1 st element contains path to file.
     * Arguments without paths to file are just boolean values,
     * where True says that argument is being used, and False
     * says, that it isn't
     * 
     * Structure also contains implicit values of paths, that
     * can be changed
     * 
     */
    class InputParameters{
        public $directory = array(TRUE,".");
        public $recursive = FALSE;
        public $parse_script = array(FALSE,"parse.php");
        public $int_script = array(FALSE,"interpreter.py");
        public $parse_only = FALSE;
        public $int_only = FALSE;
        public $jexamxml = array(FALSE,"/pub/courses/ipp/jexamxml/jexamxml.jar");
        public $jexamcfg = array(FALSE,"/pub/courses/ipp/jexamxml/options");
        public $used_by_user;
    }

    /**
     * Structure-like class containing exit codes
     */
    class ErrNums{
        const OK = 0; //No errors
        const BAD_PARAMETERS = 10; //Wrong parameters passed
        const FILE_IN_ERR = 11; //Input file error (opening,reading,etc)
        const FILE_OUT_ERR = 12; //Output file error (opening,writing,etc)
        const PARAMETER_FILE_ERR = 41; //File passed in input parameters error (existence,no permissions)
        const INNER_ERR = 99; // Inner error
    }

    /**
     * Structure-like class containing paths to testcase files and it's name
     * .rc = ref return code
     * .src = source code
     * .in = input to program
     * .out = ref output of program
     */
    class TestCasePaths{
        public $rc;    //path to .rc file
        public $src;   //path to .src file
        public $in;    //path to .in file
        public $out;   //path to .out file
        public $name;  //name of test case
    }

    /**
     * Structure-like class containing results of executed test
     * $is_ok - determines if test was success
     * $ref_retval - reference return code
     * $real_retval - real return code
     * $name - name of testcase
     * $real_output - stdout output of tested program
     * $ref_output -  reference stdout output
     * $path - path to testcase
     */
    class TestCaseResult{
        public $is_ok;
        public $ref_retval;
        public $real_retval;
        public $name;
        public $real_output;
        public $ref_output;
        public $path;
    }
?>