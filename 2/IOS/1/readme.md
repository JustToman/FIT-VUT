## IOS - dirgraph

## Goal
Create script, that scans recursively given directory and prints histogram containing number of files (based on filesize category), number of all files, number of folders.

## Language
Shell (POSIXLY_CORRECT)

## Execution
./dirgraph \[-i regex\] \[-n\] <directory>
  
-i (ignore) ignores all files and folders, which matches specified regex

-n (normalize) normalization of histogram to fit given output
