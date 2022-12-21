
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctype.h>
#include <map>
using namespace std;

//check if a character is an alphabetic letter..
bool isLetter(char c){
    return ((c>='a' && c<='z' ) || (c>='A' && c<='Z'));
}

vector<string> get_words(string filename){
    //open the input file
    ifstream f(filename);

    //define vector where to store words
    vector<string> words;

    //read file character by character and retrieve words
    char c;
    while (f.get(c)){
        //if the character we just read is a letter it means we have a new word
        if (isLetter(c)){
            //define string to hold our word
            string word;

            //add current lower case letter to word
            word+=tolower(c);

            //read letters until we reach EOF or a non-letter character
            while (f.get(c)){
                if (isLetter(c))
                    word+=tolower(c);
                else break;
            }

            // if word length is valid push it to our words vector
            if (word.length()>=3 && word.length()<=30)
                words.push_back(word);
        }
        //otherwise ignore
    }

    //return all the words
    f.close();
    return words;
}

void index_files(vector<string> filenames){

    //get number of files
    int nr_files = filenames.size();

    //
    map<string,vector<int> > file_loc, count;

    //go through the files and process them one by one
    for (int i=0; i<nr_files; i++) {
        //get all the words of this file
        vector<string> words = get_words(filenames[i]);

        //go through all the words
        for (int j=0; j<words.size(); j++){
            //if we already processed this word for this file
            //means we don't need to update file_loc, we just need to increase the count
            if (!file_loc[words[j]].empty() && file_loc[words[j]].back()==i) {
                count[words[j]].back()++;
            }
                //otherwise it's the first time we meet the word for this file
                //so we add the index of the file "i" to the file_loc of this word
                //and we push back "1" to the count, meaning this is the first word
            else {
                file_loc[words[j]].push_back(i);
                count[words[j]].push_back(1);
            }
        }
    }

    //build the index file
    ofstream INDEX("INDEX");

    //first print the number of files
    INDEX << nr_files << " ";

    //then print the filenames
    for (int i=0; i<nr_files; i++)
        INDEX << filenames[i] << " ";

    INDEX << "\n";

    //print the total number of unique words in all files
    INDEX << file_loc.size() << "\n";



    for (map<string,vector<int> >::iterator it = file_loc.begin(); it!=file_loc.end(); it++){
        string word = it->first;
        //first output the word  and how many files it appears in on a separate line
        INDEX << word << " " << it->second.size() << "\n";


        //then output all the file indices this word can be found ind
        for (int i=0; i<it->second.size(); i++)
            INDEX << it->second[i] << " ";


        INDEX << "\n";

        //now print how many times this word is located in these files
        vector<int> &cnt = count[word];
        for (int i=0; i<cnt.size(); i++)
            INDEX << cnt[i] << " ";

        INDEX << "\n";
    }

    INDEX.close();
}

void search(vector<string> keywords, bool to_sort){
    //check if empty keywords, then exit
    if (keywords.size()==0)
        return;

    //get number of keywords
    int nr_keywords = keywords.size();

    //open the INDEX file so that we can read the data
    ifstream INDEX("INDEX");

    //read the number of files and the filenames that were indexed
    int nr_files;
    vector<string> file_names;
    INDEX >> nr_files;

    for(int i=0; i<nr_files; i++) {
        string file_name;
        INDEX >> file_name;
        file_names.push_back(file_name);
    }


    //define vector to hold how many of the keywords does each file contain
    vector<int> nr_keywords_for_file(nr_files,0);
    //vector to store the score of each file
    vector<int> score(nr_files);

    //read all the words in the index file and check which ones are keywords
    int nr_words;
    INDEX >> nr_words;
    for (int i=0; i<nr_words; i++){
        //first read the word and in how many different files it appears in
        string word;
        int nr_files_for_word;
        INDEX >> word >> nr_files_for_word;

        vector<int> file_loc(nr_files_for_word), count(nr_files_for_word);

        //read in which files this word appears in
        for (int j=0; j<nr_files_for_word; j++)
            INDEX >> file_loc[j];

        //read in how many times it appears in each file
        for (int j=0; j<nr_files_for_word; j++)
            INDEX >> count[j];

        //check if current word is a keyword
        if (find(keywords.begin(),keywords.end(),word) != keywords.end()){
            //if yes, update for all the files that contain this word that they contain 1 more keyword
            for (int j=0; j<nr_files_for_word; j++) {
                nr_keywords_for_file[file_loc[j]]++;
                score[file_loc[j]]+=count[j];
            }
        }
    }

    //go through all the files and get the ones that contain all keywords
    vector<int> valid_files;
    for (int i=0; i<nr_files; i++)
        if (nr_keywords_for_file[i] == nr_keywords) {
            valid_files.push_back(i);
        }

    //sort the file indices with respect to score in decreasing order
    if (to_sort){
        sort(valid_files.begin(),valid_files.end(),[&score](int x, int y){
            if (score[x]==score[y]) return x<y;
            else return score[x]>score[y];});
    }

    for (int i=0; i<valid_files.size(); i++)
        cout << file_names[valid_files[i]] << "\n";
}

int main(int argc, char *argv[]) {
    // check if first argument is "index", "search"
    // convert first argument from C string to C++ string
    string operation = argv[1];

    if (operation == "index"){
        //get all filenames and index the files
        vector<string> filenames;
        for (int i=2; i<argc; i++)
            filenames.push_back(argv[i]);

        index_files(filenames);
    }
    else if (operation =="search"){
        //first check if we have the -p option
        bool to_sort = false;

        //start maintains the index of the first keyword in the arguments
        int start = 2;

        string arg = argv[2];

        //if the 3rd argument is -p we update "to_sort" and then we update start so that we read correctly the keywords
        if (arg == "-p") {
            to_sort = true;
            start = 3;
        }

        //get all filenames
        vector<string> keywords;
        for (int i=start; i<argc; i++){
            //get keyword
            string keyword = argv[i];

            //transform it to lowercase
            for (int j=0; j<keyword.length(); j++)
                keyword[j] = tolower(keyword[j]);

            //check if keyword length is valid then push it to the vector of all keywords
            if (keyword.length()>=3 && keyword.length()<=30)
                keywords.push_back(keyword);


        }
        search(keywords,to_sort);


    }
    else cerr << "usage: docdb index filename_1 [filename_2 ...] / docdb search keyword_1 [keyword_2 ...]";

    return 0;
}
