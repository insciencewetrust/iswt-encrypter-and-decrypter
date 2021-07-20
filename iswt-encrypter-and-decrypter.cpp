#include <fstream>
#include <sstream>
#include <vector>
#include "randgen.h"
#include "strutils.h"
using namespace std;

vector<int> optionReceiver(){
    vector<int> options;
    string o1s, o2s;
    int o1, o2;
    bool stillRunning=true;
    while (stillRunning==true){
        cout << "1) Encrypt a message" << endl << "2) Decrypt a message" << endl << "Please enter the option you want to choose (you must enter the number): ";
        cin >> o1s;
        o1=atoi(o1s);
        options.push_back(o1);
        if (o1==1){
            cout << "1) Generate a random key file" << endl << "2) Use an already existing key file" << endl << "Please enter the option you want to choose (you must enter the number): ";
            cin >> o2s;
            o2=atoi(o2s);
            options.push_back(o2);
            if (o2==1){
                stillRunning=false;
            }else if (o2==2){
                stillRunning=false;
            }else{
                cout << "Incorrect option number. Please try again." << endl;
            }
        }else if (o1==2){
            stillRunning=false;
        }else{
            cout << "Incorrect option number. Please try again." << endl;
        }
    }
    return options;
}

int RandNumGen(int sizeM1){
    RandGen r;
    int randint=r.RandInt(sizeM1);
    return randint;
}

vector<string> SymAlbetListGen(){
    vector<string> SymAlbetList;
    for (int i=32; i<127; i++){
        char sym=i;
        string sSym="";
        sSym+=sym;
        SymAlbetList.push_back(sSym);
    }
    return SymAlbetList;
}

bool multipleOccurenceChecker(int randint, vector<int> randints){
    bool multipleOccurence=false;
    if (randints.size()>0){
        for (int i=0; i<randints.size(); i++){
            if (randint==randints[i]){
                multipleOccurence=true;
            }else{
                ;
            }
        }
    }else{
        ;
    }
    return multipleOccurence;
}

void RandKeyGen(vector<string> SymAlbetList){ //Sth wrong here?
    ofstream key;
    vector<string> primKey;
    vector<int> randints;
    for (int a=0; a<SymAlbetList.size(); a++){
        bool multipleOccurence;
        int randint=RandNumGen(SymAlbetList.size());
        multipleOccurence=multipleOccurenceChecker(randint, randints);
        while (randint==a || multipleOccurence==true){
            randint=RandNumGen(SymAlbetList.size());
            multipleOccurence=multipleOccurenceChecker(randint, randints);
        }
        randints.push_back(randint);
        string strTBadded="";
        strTBadded+=SymAlbetList[a];
        strTBadded+=SymAlbetList[randint];
        primKey.push_back(strTBadded);
    }
    key.open("key.txt");
    for (int b=0; b<primKey.size(); b++){
        string unitStr="";
        unitStr+=primKey[b][0];
        unitStr+=": ";
        unitStr+=primKey[b][1];
        if (b==primKey.size()-1){
            key << unitStr;
        }else{
            key << unitStr << endl;
        }
    }
    key.close();
}

string Encrypter(string msg, string keyfileName){
    ifstream key;
    vector<string> symbols, randEncSyms;
    vector<int> orders;
    string encryptedMsg, lineKey, orgSym, newSym;
    for (int a=0; a<msg.length(); a++){
        string strChr="";
        strChr+=msg[a];
        symbols.push_back(strChr);
    }
    for (int b=0; b<symbols.size(); b++){
        encryptedMsg+="a";
    }
    key.open(keyfileName.c_str());
    while (getline(key, lineKey)){
        istringstream eltsKey(lineKey);
        eltsKey >> orgSym >> newSym;
        orgSym.pop_back();
        for (int c=0; c<symbols.size(); c++){
            if (orgSym==symbols[c]){
                orders.push_back(c);
                randEncSyms.push_back(newSym);
            }else{
                ;
            }
        }
    }
    key.close();
    for (int d=0; d<randEncSyms.size(); d++){
        encryptedMsg[orders[d]]=randEncSyms[d][0];
    }
    return encryptedMsg;
}

string Decrypter(string encryptedMsg, string keyfileName){
    ifstream key;
    vector<string> symbols, randDecSyms;
    vector<int> orders;
    string decryptedMsg, lineKey, orgSym, newSym;
    for (int a=0; a<encryptedMsg.length(); a++){
        string strChr="";
        strChr+=encryptedMsg[a];
        symbols.push_back(strChr);
    }
    for (int b=0; b<symbols.size(); b++){
        decryptedMsg+="a";
    }
    key.open(keyfileName.c_str());
    while (getline(key, lineKey)){
        istringstream eltsKey(lineKey);
        eltsKey >> orgSym >> newSym;
        orgSym.pop_back();
        for (int c=0; c<symbols.size(); c++){
            if (newSym==symbols[c]){
                orders.push_back(c);
                randDecSyms.push_back(orgSym);
            }else{
                ;
            }
        }
    }
    key.close();
    for (int d=0; d<randDecSyms.size(); d++){
        decryptedMsg[orders[d]]=randDecSyms[d][0];
    }
    return decryptedMsg;
}

int main(){
    vector<int> options=optionReceiver();
    if (options.size()==2){
        if (options[1]==1){
            vector<string> SymAlbetList=SymAlbetListGen();
            RandKeyGen(SymAlbetList);
            string msg, encryptedMsg;
            cout << "A random key file has been generated under the name \"key.txt\". You can use that file later to encrypt or decrypt messages." << endl;
            cout << "Please enter the message you want to encrypt: ";
            cin.ignore();
            getline(cin, msg);
            encryptedMsg=Encrypter(msg, "key.txt");
            cout << "The encrypted message is: " << encryptedMsg << endl;
        }else{
            string keyfileName, msg, encryptedMsg;
            cout << "Please enter the name (include the \".txt\" part) of the key file you want to use for encryption: ";
            cin >> keyfileName;
            cout << "Please enter the message you want to encrypt: ";
            cin.ignore();
            getline(cin, msg);
            encryptedMsg=Encrypter(msg, keyfileName.c_str());
            cout << "The encrypted message is: " << encryptedMsg << endl;
        }
    }else{
        string keyfileName, encryptedMsg, decryptedMsg;
        cout << "Please enter the name (include the \".txt\" part) of the key file you want to use for decryption: ";
        cin >> keyfileName;
        cout << "Please enter the encrypted message: ";
        cin.ignore();
        getline(cin, encryptedMsg);
        decryptedMsg=Decrypter(encryptedMsg, keyfileName);
        cout << "The decrypted message is: " << decryptedMsg << endl;
    }
    return 0;
}