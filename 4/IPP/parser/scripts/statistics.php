<?php

    /**
     * This classed is supposed to work as c-like structure containing info about
     * one statistic file and it's parameters
     */
    class StatisticsPermissions{
        public $file;          //File handler
        public $order;         //Array Meaning key => value pairs with keys 0,1,2,3 => '--loc', '--labels',(and other parameters)
    }
    class Statistics{

        //Passed arguments
        private $permissions_array; //Array containing


        //Counters
        private $n_of_lines = 0;
        private $n_of_comments = 0;
        private $n_of_labels = 0;
        private $n_of_jumps = 0;
        private $n_of_fw_jumps = 0;
        private $n_of_back_jumps = 0;
        private $n_of_bad_jumps = 0;


        /**
         * @param StatisticsPermissions[] array containing info about statistic parameters
         */
        public function __construct($permissions_array){
            
            $this->permissions_array = $permissions_array;            
        }

        /**
         * Increments $n_of_lines counter by specificied value
         * @param int $n_of_lines specifies increment size (implicit 1)
         */
        public function incNumberOfLines($n_of_lines = 1){
            $this->n_of_lines += $n_of_lines;
        }

        /**
         * Increments $n_of_comments counter by specificied value
         * @param int $n_of_comments specifies increment size (implicit 1)
         */
        public function incNumOfComments($n_of_comments = 1){
            $this->n_of_comments += $n_of_comments;
        }

        /**
         * Increments $n_of_labels counter by specificied value
         * @param int $n_of_labels specifies increment size (implicit 1)
         */
        public function incNumOfLabels($n_of_labels = 1){
            $this->n_of_labels += $n_of_labels;
        }

        /**
         * Increments $n_of_jumps counter by specificied value
         * @param int $n_of_jumps specifies increment size (implicit 1)
         */
        public function incNumOfJumps($n_of_jumps = 1){
            $this->n_of_jumps += $n_of_jumps;
        }

        /**
         * Increments $n_of_fw_jumps counter by specificied value
         * @param int $n_of_fw_jumps specifies increment size (implicit 1)
         */
        public function incNumOfForwardJumps($n_of_fw_jumps = 1){
            $this->n_of_fw_jumps += $n_of_fw_jumps;
        }

        /**
         * Increments $n_of_back_jumps counter by specificied value
         * @param int $n_of_back_jumps specifies increment size (implicit 1)
         */
        public function incNumOfBackJumps($n_of_back_jumps = 1){
            $this->n_of_back_jumps += $n_of_back_jumps;
        }

        /**
         * Increments $n_of_bad_jumps counter by specificied value
         * @param int $n_of_bad_jumps specifies increment size (implicit 1)
         */
        public function incNumOfBadJumps($n_of_bad_jumps = 1){
            $this->n_of_bad_jumps += $n_of_bad_jumps;
        }

        /**
         * Writes statistics to file depending on $permissions_array elements 
         * (StatisticsPermissions structure)
         * 
         * 
         * @return bool TRUE if writing was successful, FALSE if it wasn't
         */
        public function writeStatistic(){
            /*Iterates through whole permissions array */
            foreach($this->permissions_array as $element){
                /* Then iterates through whole order array
                   depending on parameter value, prints counter
                   values to file specified by $element->file 
                */
                for($i = 0; $i < count($element->order); $i = $i + 1){
                   $is_ok = TRUE;
                    switch($element->order[$i]){
                        case "--loc":
                            $is_ok = fwrite($element->file, strval($this->n_of_lines)."\n");
                            break;
                        case "--comments":
                            $is_ok = fwrite($element->file, strval($this->n_of_comments)."\n");
                            break;
                        case "--labels":
                            $is_ok = fwrite($element->file, strval($this->n_of_labels)."\n");
                            break;
                        case "--jumps":
                            $is_ok = fwrite($element->file,strval($this->n_of_jumps)."\n");
                            break;
                        case "--fwjumps":
                            $is_ok = fwrite($element->file,strval($this->n_of_fw_jumps)."\n");
                            break;
                        case "--backjumps":
                            $is_ok = fwrite($element->file,strval($this->n_of_back_jumps)."\n");
                            break;
                        case "--badjumps":
                            $is_ok = fwrite($element->file,strval($this->n_of_bad_jumps)."\n");
                            break;
                        default:
                            echo "FALSE";
                            return FALSE;
                    }
                    if(!$is_ok){
                        return FALSE;
                    }
                }
            }
            return TRUE;
        }
    }
?>