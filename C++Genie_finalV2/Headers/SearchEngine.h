#include"DirectoryHandler.h"
#include"FileHandler.h"
//#include"ErrorWindow.h"

const int MAX_WORDS_NO = 10;
const int WORD_PRIORITY = 3;
const int MAX_WORD_LEN = 20;
const int MAX_COMP_INCOS_STR_LEN = 2;
const char SYNONYM_DIR[] = ".\\synonym\\";

using namespace std;
DisplayInfo searchEngineDi;
class SearchEngine :public DirectoryHandler
{
private:
    int priority1[MAX_FILE_NO][MAX_WORDS_NO] ={{0}};  //store occurances of consecutive matched keywords for each file
    int priority2[MAX_FILE_NO][MAX_WORDS_NO] ={{0}};   //store occurances of matched keywords for each file
    const char* priorityList[MAX_FILE_NO];                 //priority list based on priority1 and priority2 for files
    const string* keywords;
    const string* synonyms[MAX_WORDS_NO];     //synonym words of given keyword

public:
    void searchForResult(const char *keyword);
    int readSynonyms(const char *keyword , int index);
    string* breakIntoWords(const char *str);

   int countInconsistent(const char* c1, const char* c2)
    {
        int inconsistentCount = 0;
        for(unsigned int j=0;j<string(c2).size();j++)
        {
            if(*(c1+j) != *(c2+j))
            {
                inconsistentCount++;
            }
        }
        return inconsistentCount;
    }

   int findNoOfWords(const char *str)
   {
       int i=0,wordCount = 1;
       while(*(str+i) != '\0')
       {
            if(isPunctuation(*(str+i)) && !isPunctuation(*(str+i-1)) )
                wordCount++;
            i++;
       }
       return wordCount;
   }

   bool isPunctuation(const char l)
   {
       if((l>= 32 && l<=47) || (l>=58 && l<=64)|| (l>=91 && l<=96)|| (l>=123 && l<=126) || l=='\0' || l == '\n')
            return true;
       else
            return false;
   }
};

    int SearchEngine::readSynonyms(const char* keyword ,int index)
    {
        string synfilepath = string(SYNONYM_DIR) + keyword[0] + string("\\") + keyword + string(".txt");
        FileHandler synfile;
        synfile.setFilePath(synfilepath.data());
        int synCount = 0;

        string tempSynword("");
        if(synfile.exists())
        {
            ifstream synfilein;
            synfilein.open(synfilepath.data(),ios::in);
            int i = 0;
            while(!synfilein.eof())
            {
                synfilein.get(tempSynword[i]);
                i++;
            }
            tempSynword[i-1] = '\0';
            synCount = findNoOfWords(tempSynword.data()) -1;
            synonyms[index] = breakIntoWords(tempSynword.data());
        }
        return synCount;
    }

    string* SearchEngine::breakIntoWords(const char *str)
   {
       string *words;
       string str1(str);
       words = new string[findNoOfWords(str)];
       char word[MAX_WORD_LEN];
       char l;

       for(unsigned int i = 0,j=0,k=0;i<=str1.size();i++)
       {
           l = tolower(str1[i]);
           word[k] = l;
           k++;

           if(isPunctuation(l) || l=='\0')
            {
              if(i>0)
              {
                word[k-1] = '\0';
                k = 0;
                words[j] = string(word);
                j++;
              }
              else
              {
                delete[] words;
                char altstr[MAX_WORD_LEN];
                for(unsigned int p=0; p<(str1.size()-1);p++)
                {
                   altstr[p] = str1[p+1];
                }
                altstr[str1.size()-1] = '\0';
                return breakIntoWords(altstr);
              }
            }
        }
       return words;
   }


   void SearchEngine::searchForResult(const char *keyword)
   {
        int wordNo = findNoOfWords(keyword);   //no of words in given keyword
        bool foundword[wordNo];    //found given keyword
        bool foundAltWord[WORD_PRIORITY][wordNo];  //found alternative words to given keywords
        bool checkFA[WORD_PRIORITY][wordNo];    //check for alternatives for given keywords
        bool foundCW[wordNo+1];      //found consecutive words matching the given keyword
        int FCW = 0;      //no of found consecutive matched words
        char word[MAX_WORD_LEN];
        char altWord[WORD_PRIORITY][wordNo][MAX_WORD_LEN];
        int synonymCount[MAX_WORDS_NO] = {0};

        string completeFileName;
        string returninfo;

        fill(foundword,(foundword+wordNo),false);
        fill(foundCW,(foundCW+wordNo+1),false);
        memset(foundAltWord, false, sizeof(bool) * wordNo * WORD_PRIORITY);
        memset(checkFA, true, sizeof(bool) * wordNo * WORD_PRIORITY);

        keywords = breakIntoWords(keyword);  //break the given keyword into words

        for(int ksc = 0;ksc <wordNo;ksc++) //KSC = keyword string count
        {
            synonymCount[ksc] = readSynonyms(keywords[ksc].data(),ksc);
            if(synonymCount[ksc] > 0)
            {
                returninfo = string("Including searches for synonym of ") + keywords[ksc] + string(" as:");
                searchEngineDi.add(returninfo);
                for(int sc = 0;sc<synonymCount[ksc] ; sc++)              //sc - synonym count or no of synonyms of current keyword
                {
                  returninfo = string("(") + synonyms[ksc][sc] + string(")");
                  searchEngineDi.add(returninfo);
                }

            }
        }

        ifstream filein;

        char letter;
        int letterCount = 0;
        string curKeyword;

        for(int fileCount=0; fileCount<fileNo ; fileCount++)
        {
            if(getFileExtension(filename[fileCount]) != ".txt")
                continue;
            completeFileName = (string(dirPath)+filename[fileCount]);
            filein.open(completeFileName.data(),ios::in);
            while(!filein.eof())
            {
                filein >>noskipws>> letter;
                //cout<< letter;
                letter = tolower(letter);
                word[letterCount]=letter;
                letterCount++;
                    if(isPunctuation(letter))
                    {
                        word[letterCount-1]='\0';
                        for(int ksc = 0;ksc <wordNo;ksc++) //ksc = keyword string count
                        {
                            if(synonymCount[ksc]>0)
                            {
                            for(int sc= 0;sc<synonymCount[ksc];sc++)
                            {
                                string synKeyword = synonyms[ksc][sc];
                                if((string(word) == synKeyword))
                                {
                                foundword[ksc] = true;
                                foundCW[0] = true;
                                FCW++;
                                priority1[fileCount][1]++;
                                for(int cw =1;cw<=wordNo;cw++)
                                {
                                    if(FCW >= cw)
                                    {
                                        foundCW[FCW] = true;
                                        priority1[fileCount][FCW]++;
                                    }
                                    else
                                        break;
                                }

                                priority2[fileCount][ksc] +=1;
                                for(int w=0;w<=WORD_PRIORITY ;w++)
                                {
                                    checkFA[w][ksc] = false;
                                }
                                }
                            }
                           }
                            curKeyword = keywords[ksc];

                           if((string(word) == curKeyword))
                            {
                                foundword[ksc] = true;
                                foundCW[0] = true;
                                FCW++;
                                priority1[fileCount][1]++;
                                for(int cw =1;cw<=wordNo;cw++)
                                {
                                    if(FCW >= cw)
                                    {
                                        foundCW[FCW] = true;
                                        priority1[fileCount][FCW]++;
                                    }
                                    else
                                        break;
                                }

                                priority2[fileCount][ksc] +=1;
                                for(int w=0;w<=WORD_PRIORITY ;w++)
                                {
                                    checkFA[w][ksc] = false;
                                }
                             }


                            else if(checkFA[0][ksc] && curKeyword.size() >MAX_COMP_INCOS_STR_LEN && (curKeyword.size() == string(word).size()))
                            {  //check for alternatives to the current keyword
                                if(countInconsistent(word,curKeyword.data()) == 1)
                                {
                                    strncpy(altWord[0][ksc], word,string(word).size());
                                    altWord[0][ksc][string(word).size()] = '\0';
                                    foundAltWord[0][ksc] = true;
                                    for(unsigned int w=0;w<=WORD_PRIORITY ;w++)
                                    {
                                        checkFA[w][ksc] = false;
                                    }
                                }
                            }

                            else if(checkFA[1][ksc] && curKeyword.size()>MAX_COMP_INCOS_STR_LEN && ((curKeyword.size()+1) == string(word).size()))
                            {
                                for(unsigned int j =0; j<curKeyword.size() && !foundAltWord[1][ksc];j++)
                                {
                                    altWord[1][ksc][j] = 'a';
                                    for(unsigned int k = 0;k<j;k++)
                                    {
                                        altWord[1][ksc][k] = curKeyword[k];
                                    }
                                    for(unsigned int k = j;k<curKeyword.size();k++)
                                    {
                                        altWord[1][ksc][k+1] = curKeyword[k];
                                    }
                                    if(countInconsistent(altWord[1][ksc],word) <= 1)
                                    {
                                        strcpy(altWord[1][ksc], word);
                                        altWord[1][ksc][string(word).size()] = '\0';
                                        foundAltWord[1][ksc] = true;
                                        for(int w=1;w<=WORD_PRIORITY ;w++)
                                        {
                                            checkFA[w][ksc] = false;
                                        }
                                    }
                                }
                            }

                            else if(checkFA[2][ksc] && curKeyword.size()>MAX_COMP_INCOS_STR_LEN && ((curKeyword.size()-1) == string(word).size() ))
                            {
                                for(unsigned int j =0; j<(curKeyword.size()-1) && !foundAltWord[2][ksc];j++)
                                {
                                    for(unsigned int k = 0;k<j;k++)
                                    {
                                        altWord[2][ksc][k] = curKeyword[k];
                                    }
                                    for(unsigned int k = j;k<(curKeyword.size()-1);k++)
                                    {
                                        altWord[2][ksc][k] = curKeyword[k+1];
                                    }
                                    altWord[2][ksc][curKeyword.size()-1] = '\0';
                                    if(countInconsistent(word,altWord[2][ksc]) <= 1)
                                    {
                                        strcpy(altWord[2][ksc], word);
                                        foundAltWord[2][ksc] = true;
                                    }
                                }
                            }
                        }
                        if(!foundCW[0] && (FCW != 0))
                        {
                            FCW = 0;
                        }
                        letterCount = 0;
                        foundCW[0] = false;
                    }
            }

            filein.close();
        }

        for(int ksc = 0;ksc <wordNo;ksc++) //ksc = keyword string count
            {
                if(!foundword[ksc])
                {
                    if(foundAltWord[0][ksc])
                    {
                    returninfo = string("Did you mean \"")+altWord[0][ksc] + string("\" for \"") + keywords[ksc] + string("\"?");
                    searchEngineDi.add(returninfo);
                    }
                    else if(foundAltWord[1][ksc])
                    {
                    returninfo = string("Did you mean \"")+altWord[1][ksc]+string("\" for \"")+keywords[ksc]+string("\"?");
                    searchEngineDi.add(returninfo);
                    }
                    else if(foundAltWord[2][ksc])
                    {
                      returninfo = string("Did you mean \"")+altWord[2][ksc]+string("\" for \"")+keywords[ksc]+string("\"?");
                      searchEngineDi.add(returninfo);
                    }
                    else
                    {
                        returninfo = string("Sorry, couldn't find any result for ") + keywords[ksc];
                        searchEngineDi.add(returninfo);
                    }
                }
                else{
                    for(int fc =0;fc<fileNo;fc++)  // fc = file count
                    {
                    if(getFileExtension(filename[fc]) != ".txt")
                        continue;
                    returninfo = string("Occurances of \"")+keywords[ksc]+string("\" for file: \"")+filename[fc]+string("\" is ")+ to_string(priority2[fc][ksc]);
                    searchEngineDi.add(returninfo);
                    }
                }
            }
            FCW = 0;
            for(int cw=1;cw<=wordNo;cw++)
            {
                if(foundCW[cw])
                    FCW++;
            }
            if(wordNo>1)
            {
            returninfo = to_string(FCW)+string(" words out of given ")+to_string(wordNo)+string(" were found to occur consecutively");
            searchEngineDi.add(returninfo);

            for(int fc=0;fc<fileNo;fc++)
            {
                for(int ksc=wordNo-1;ksc>1;ksc--)
                {
                    if(priority1[fc][ksc]>0)
                    {
                        returninfo = to_string(priority1[fc][ksc]) + string(" Occurances of ")+to_string(ksc)+string(" words out of given ")+to_string(wordNo)+string(" were found to occur consecutively in file ")+filename[fc];
                        searchEngineDi.add(returninfo);
                        break;
                    }
                }
            }
            }

        delete[] keywords;

        for(int ksc = 0;ksc <wordNo;ksc++)
        {
            if(synonymCount[ksc]>0)
                delete[] synonyms[ksc];
        }
   }

   class Synonym
   {
   protected:
     string synword1;  // saves synonym word 1 entered by user for references to future search for keywords
     string synword2;  // saves synonym word 2 entered by user for references to future search for keywords
     DirectoryHandler syndir;
     FileHandler synfile;
     ofstream fileout;
    public:
     void getInput(const char* syn1, const char* syn2)
     {
         synword1 = syn1;
         synword2 = syn2;
     }

     void addToFile()
     {
         syndir.setDirName(SYNONYM_DIR);
         syndir.hidedir();

        string syndir1 = string(SYNONYM_DIR) + synword1[0];
        syndir.setDirName(syndir1.data());
        if(!syndir.exists())
            syndir.createDir();
        syndir1 = string(SYNONYM_DIR) + synword1[0] + string("\\") + synword1 + string(".txt");
        synfile.setFilePath(syndir1.data());
        if(!synfile.exists())
            synfile.createFile();
        fileout.open(syndir1.data(), ios::app);
        fileout<<synword2<<"\n";
        fileout.close();

        string syndir2 = string(SYNONYM_DIR) + synword2[0];
        syndir.setDirName(syndir2.data());
        if(!syndir.exists())
            syndir.createDir();
        syndir2 = string(SYNONYM_DIR) + synword2[0] + string("\\") + synword2 + string(".txt");
        synfile.setFilePath(syndir2.data());
        if(!synfile.exists())
            synfile.createFile();
        fileout.open(syndir2.data(), ios::app);
        fileout<<synword1<<"\n";
        fileout.close();
     }
   };

void SetSynonym(const char* syn1, const char* syn2)
{
    Synonym syn;
    syn.getInput(syn1,syn2);
    syn.addToFile();
}

void Search(const char* key,const char* dirpath)
{
    SearchEngine s;
        s.setDirName(dirpath);
        s.setDirForUse();

    if(s.isValid)
    {
    s.displayfile();
    s.searchForResult(key);
    }
    else
    {
        searchEngineDi.add("No files to search from!!");
    }
}


