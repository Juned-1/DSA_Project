#include"accessfile.hpp"
using namespace std;
void toLowerCase(char * word){
    int size = (int)strlen(word);
    for(int i = 0; i < size; i++){
        word[i] = tolower(word[i]);
    }
}
char * subString(char * buffer, int start, int end){
    int size = end - start - 1;
    char * word = new char[size+1];
    for(int i = start+1; i < end; i++){
        word[i-start-1] = buffer[i];
    }
    word[size] = '\0';
    return word;
}
vector<string> getFileList(string folder)
{
    vector<string> names;
    string search_path = folder + "/*.*";
    WIN32_FIND_DATA fd;
    HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
    if(hFind != INVALID_HANDLE_VALUE) {
        do {
            // read all (real) files in current folder
            // , delete '!' read other 2 default folder . and ..
            if(! (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ) {
                names.push_back(fd.cFileName);
            }
        }while(::FindNextFile(hFind, &fd));
        ::FindClose(hFind);
    }
    return names;
}
void accessOperation(){
    ofstream output;
    output.open("output.txt",std::ios::out); //opening file in write mode
    char currentDirectory[PATH_MAX]; //maximum size of path depending on system
    _getcwd(currentDirectory,PATH_MAX); //for windows
    //getcwd(currentDirectory,PATH_MAX); for linux -- header uinstd
    strcat(currentDirectory,"\\file"); //current directory with folder file
    int stop = (int)strlen(currentDirectory);

    DIR * dir = opendir(currentDirectory);
    if(dir == NULL){
        cout<<"#######################################################################"<<endl;
        cout<<"# There is no directory called file in your Present Working Directory #"<<endl;
        cout<<"#           Make sure to create a directory namely \'file\'           #"<<endl;
        cout<<"#                 And store all document dataset in it                #"<<endl;
        cout<<"#######################################################################"<<endl;
        cout<<"BYE BYE!"<<endl<<endl<<endl;

        output<<"#######################################################################"<<endl;
        output<<"# There is no directory called file in your Present Working Directory #"<<endl;
        output<<"#           Make sure to create a directory namely \'file\'           #"<<endl;
        output<<"#                 And store all document dataset in it                #"<<endl;
        output<<"#######################################################################"<<endl;
        output<<"BYE, BYE!"<<endl<<endl<<endl;

        exit(0);
    }

    vector<string> fileList = getFileList(currentDirectory);

    if(fileList.empty()){
        cout<<"#####################################################"<<endl;
        cout<<"#           There is no file to read                #"<<endl;
        cout<<"# Please make sure to include file to rank document #"<<endl;
        cout<<"#####################################################"<<endl;
        cout<<"BYE, BYE!"<<endl<<endl<<endl;

        output<<"#####################################################"<<endl;
        output<<"#           There is no file to read                #"<<endl;
        output<<"# Please make sure to include file to rank document #"<<endl;
        output<<"#####################################################"<<endl;
        output<<"BYE, BYE!"<<endl<<endl<<endl;

        exit(0);
    }

    //Getting the keyword from useraccording to which document will be ranked
    char * keyword = new char[150];
    cout<<"[PLEASE ENTER THE KEYWORD]:"<<endl;
    output<<"[KEYWORD ACCORDING TO WHICH DOCUMENT IS RANKED]:"<<endl;
    cout<<"=>";
    output<<"=>";
    cin>>keyword;
    output<<keyword<<endl;
    toLowerCase(keyword);
    cout<<endl<<endl;
    output<<endl<<endl;

    //Since we need most frequent word we use MAX-HEAP
    BinomialHeap priorityQueue;
    //priorty queue of STL type queue<document>

    //Reading each document one by one
    for(const auto & it: fileList){
        document * doc = new document;
        doc->name = (char *)it.c_str();
        //cout<<doc.name<<endl;
        if(doc->name[0] == '.'){continue;}
        //If the file open with . then it is not content file and should not open it
        strcat(currentDirectory,"\\");
        strcat(currentDirectory,doc->name);
        ifstream input;
        input.open(currentDirectory,ios::in);
        currentDirectory[stop] = '\0';
        if(!input){continue;}
        //file is opened

        input.seekg(0,ios_base::end); //making get pointer to point last position in file
        //ifstream thre is get pointer --to end of file(eof)
        int size = (int)input.tellg();
        //cout<<size<<endl;
        //getting the size of document -- since we rae going to read entire file, not lin eby line
        char * buffer = new char[size+1];
        input.seekg(0,ios_base::beg); //rewinding file pointer to initial point
        //cout<<input.tellg()<<endl;
        for(int i = 0; i < size; i++){
            buffer[i] = input.get();
        }
        buffer[size] = '\0';
        doc->text = buffer;

        int frequency = 0;
        //How many keywords are there in the current document?
        int start = -1, end;
        for(end = 0; end <= size; end++){
            if(!isalnum(buffer[end])&&buffer[end]!='\''){//alnum check alphanumeric defined in ctype
                //On encountering separator we divide the word as substring from sentence
                if(start+1 < end){
                    char * word = subString(buffer,start,end);
                    toLowerCase(word);
                    if(strcmp(word,keyword)==0){
                        frequency++;
                    }
                }
                start = end;
            }
        }
        priorityQueue.enqueue(frequency,doc);
        input.close();
    }

    cout<<"Priority Queue has been created"<<endl;
    //-------Relavency check
    int numberOfRelevant;
    cout<<"Now,Enter how many document do you need?"<<endl;
    cout<<"=>";
    cin>>numberOfRelevant;
    output<<"[NUMBER OF MOST RELEVANT DOCUMENT]:"<<endl;
    output<<"=> "<<numberOfRelevant<<endl<<endl;

    BinomialTree * answers[numberOfRelevant];
    for(int i = 0; i < numberOfRelevant; i++){
        //This array will help us to store most frequent document on the basis of keyword
        //So that we can print them
        answers[i] = NULL;
    }

    //Let's dequeue or pull the document
    for(int i = 0; i < numberOfRelevant; i++){
        answers[i] = priorityQueue.dequeue();
    }

    //Printing the relevance order
    cout<<"################################################################"<<endl;
    cout<<"#                    PRINTING DOCUMENT                         #"<<endl;
    cout<<"################################################################"<<endl;
    cout<<endl<<endl<<endl;
    output<<"################################################################"<<endl;
    output<<"#                    PRINTING DOCUMENT                         #"<<endl;
    output<<"################################################################"<<endl;
    output<<endl<<endl<<endl;
    for(int i = 0; i < numberOfRelevant; i++){
        cout<<i+1<<". [name of document = "<<answers[i]->doc->name<<" | frequency of keyword = "<<answers[i]->frequency<<"]"<<endl;
        output<<i+1<<". ["<<answers[i]->doc->name<<" | "<<answers[i]->frequency<<"]"<<endl;
        //cout<<answers[i]->doc->text<<endl<<endl<<endl<<endl<<endl;
        output<<answers[i]->doc->text<<endl<<endl<<endl<<endl<<endl;
    }
    output.close();
}
