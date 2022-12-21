# Coding-HSG_MACS-Group

Project's Title: 
DATABASE OF FILES

Project Description: 
The programme has the purpose of reading several text files and generating a reverse index of the words in all the files. In this way, in a subsequent invocation, the programme will be able to search the files by keywords. It may be operated with the following three sets of command line options:

• dbfiles index filename1 [filename2...]
In this case, an indexing function is executed by the programme. The programme reads the specified files and creates an index in a file called “INDEX”. The index is a reverse word index, which basically means that it maps any word to a list of files. A word can be defined as a maximum contiguous sequence of alphabetic characters adjacent to any non-alphabetic character (either at the end or at the beginning of the file). In indexing and searching, only words between 3 and 30 characters in length are addressed. The index must be compact in such a way that the list of files per word is stored as a list of short numerical file identifiers.

• dbfiles search keyword1 [keyword2...]
In this case, the search function is run by the programme. The programme returns a list of all the files (file names) that contain all the keywords passed on the command line after reading the index from a file called INDEX. It’s crucial to mention that the keyword match should be case- insensitive. No results are output if no indexed file contains all the indicated keywords. The original order passed to the indexing function is followed when printing the names of the matching files. The search function only read the index file. This means that the search function works even if the files are not available at the time the search function is invoked.

• docdb search -p keyword1 [keyword2...]
Ultimately, in this last case, the programme performs the search function as described before, but instead of sorting the output files in the original indexing order, it produces the matching files in descending order with respect to the total number of matching words. The search function gives priority to the output based on the strength of a match.

Usage:
Compile file: make dbfiles
We create a MakeFile to compile our programme and then execute it.

Note: More information on the code in question can be found in the pdf attached to the folder. 
Name file: Coding@HSG_MACS Group.pdf
