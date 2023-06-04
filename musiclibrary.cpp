//
//  Project 2 - Music Library
/// @author Victor Samuel Escudero
/// @date February 14, 2023
/// @brief Music Library App for CS-251. This app is able to load music data and display it to the screen.
///        It saves artists and songs associated with an album it allows you to search for an specific 
///         artists, song, or album as well as filter your search with the + and - modifiers. It also allows 
///         you to export the music library data to a file for personal use and clear data to keep loading more.

/// Assignment details and provided code are created and
/// owned by Adam T Koehler, PhD - Copyright 2023.
/// University of Illinois Chicago - CS 251 Spring 2023

#include <iostream>
#include <string>

using namespace std;

// INSERTS GIVEN HELPERS CODE BELOW - DO NOT REMOVE
#include "helpers.cpp"
// INSERTS GIVEN HELPERS CODE ABOVE - DO NOT REMOVE

//
// NOTE TO STUDENTS:
// You may use the functions in the helpers file,
// but you may not modify the file or its contents in any way whatsoever.
//

//
// STUDENT CODE MAY GO BELOW THIS LINE
//

#include <map>
#include <set>
#include <sstream>
#include <algorithm>

//Struct containing a set of unique artist and songs per album
struct Album{

  set<string> artist;
  set<string> songs;
};

//Function prototypes:

//load function and their respective extra functions
void storeAlbumsObjects(set<string> &, set<string> &, ifstream &, Album &, string &);
void initializeAlbumsObject(map<string, Album>  &, set<string> &, set<string> &, ifstream &, string &);
void load(string &,  map<string, Album>  &, set<string> &, set<string> &);

//stats function and their respective extra functions
void displayAlbumsObjects(string , set<string> &, set<string> &);
void stats(map<string, Album> &, set<string> &, set<string> &, string &);

//clear function
void clear(map<string, Album> &, set<string> &, set<string> &);

//export function and their respective extra functions
void exportAlbumObjects(ofstream &, string , set<string> &, set<string> &);
void exportLibrary(map<string, Album> &, string &);

//search function and their respective extra functions
void findSearchTerm(string &, string , string &, set<string> &);
void loopThroghAlbum(set<string> &, set<string> &, string &, string &);
void updateSets(set<string> &, set<string> &, set<string> &, char &);
void pickObjectsLoop(map<string, Album> & , set<string> &, string &, string &);
void getResultingSet(map<string, Album> & , set<string> &, set<string> &, set<string> &, string &, string &);
void search(map<string, Album> &, string &);


int main()
{
    string userEntry;
    string command, remains;

    //Map containing all music data
    map<string, Album> musicLibrary;

    //sets containing current artist and songs in our library
    set<string> uniqueArtists;
    set<string> songsInLibrary;

    // Display welcome message once per program execution
    cout << "Welcome to the Music Library App" << endl;
    cout << "--------------------------------" << endl;

    // Stay in menu until exit command
    do
    {
        cout << endl;
        cout << "Enter a command (help for help): " << endl;
        getline(cin, userEntry);
        cout << endl;

        //Split the user entry into two pieces
        splitFirstWord(userEntry, command, remains);
        tolower(command);

        // take an action, one per iteration, based on the command
        if (command == "help")
        {
            helpCommand();
        }
        else if (command == "clear")
        {
            clear(musicLibrary, uniqueArtists, songsInLibrary);
        }
        else if (command == "export")
        {
            //string exportFilename = remains;//make a copy of 
            exportLibrary(musicLibrary, remains);
        }
        else if (command == "load")
        {
            load(remains, musicLibrary, uniqueArtists, songsInLibrary);
        }
        else if (command == "stats")
        {
            //string flag = remains;
            stats(musicLibrary, uniqueArtists, songsInLibrary, remains);
        }          
        else if (command == "search")
        {
            search(musicLibrary, remains);
        }

    }while(command != "exit");    

    cout << "Thank you for using the Music Library App" << endl;
    return 0;
}
//---------------------------------------------------------------------------------------------------------------
/// @brief using the data string it checks what category does the data fall under and stores it in the correct
///         spot. This data can be songs or artist. The function inserts data in the correct set and reads next line
/// @param uniqueArtists a set of strings containing all unique artists in our current library
/// @param songsInLibrary a set of strings containing all unique songs in our library 
/// @param inFile an ifstream containing all the loaded data from a file
/// @param albumObject a struct of type Album containing either a song or artist 
/// @param data a string containing the input file line
void storeAlbumsObjects(set<string> &uniqueArtists, set<string> &songsInLibrary, ifstream &inFile, 
Album &albumObject, string &data)
{
    //Pick whether our data correspond to and artist or song category and store accordinlgy
    if(isdigit(data[0]))
    {
        albumObject.songs.insert(data);
        songsInLibrary.insert(data);
    }else 
    {
        albumObject.artist.insert(data);
        uniqueArtists.insert(data);
    }

    getline(inFile, data);
}
//---------------------------------------------------------------------------------------------------------------
/// @brief the function reads album data if the album has not been yet stored. If it is a new album it goes
///         retrieves all data pertaining to one specific album and stores it into our map
/// @param musicLibrary a map of structs of type Album containing all album related information
/// @param uniqueArtists a set of strings containing all unique artists in our current library
/// @param songsInLibrary a set of strings containing all unique songs in our library 
/// @param inFile an ifstream containing all the loaded data from a file
/// @param albumKey a string containg the current album we are reading
void initializeAlbumsObject(map<string, Album>  &musicLibrary, set<string> &uniqueArtists, set<string> &songsInLibrary, 
ifstream &inFile, string &albumKey)
{
    string value;
    Album albumObject;
    //If album already exist in our library skip info until the next album
    if(musicLibrary.count(albumKey))
    {
        getline(inFile, value);
        while(value != albumKey)//skip till next album
        {
            getline(inFile, value);
        }
    
    }else//if it does not exist in our library store data
    {
    
        getline(inFile, value);

        //loop through all the albums content
        while(value != albumKey)
        {
            storeAlbumsObjects(uniqueArtists, songsInLibrary, inFile, albumObject, value);
        }

        musicLibrary[albumKey] = albumObject;//add our new data to our map

    }

}
//---------------------------------------------------------------------------------------------------------------
/// @brief using the file provided it loads and stores music data info into our map. It also stores unique artist and songs 
///         into their own respective sets for display purposes. Function will use music.txt file by default if no file was provided
/// @param file a string containing the name of the file we are going to load from
/// @param musicLibrary a map of structs of type Album containing all album related information
/// @param uniqueArtists a set of strings containing all unique artists in our current library
/// @param songsInLibrary a set of strings containing all unique songs in our library 
void load(string &file,  map<string, Album>  &musicLibrary, set<string> &uniqueArtists, set<string> &songsInLibrary)
{
    string albumKey = "";

    if(file == "")
    {
        file = "music.txt";
    }
  
    ifstream inFile(file);
    if(inFile.fail())
    {
        cout << "Error: Could not open music library file - " << file << endl;
        return;
    }

    //extract data until end of file
    while(getline(inFile, albumKey))
    {
        initializeAlbumsObject(musicLibrary, uniqueArtists, songsInLibrary, inFile, albumKey);
    }

    inFile.close();
    
}
//---------------------------------------------------------------------------------------------------------------
/// @brief the function displays the set of artist and songs in an album
/// @param albumName a string containg the current album we are looping through
/// @param artistsSet a set of strings containing all unique artist for one album
/// @param songsSet a set of strings containing all unique songs for one album
void displayAlbumsObjects(string albumName, set<string> &artistsSet, set<string> &songsSet)
{
    cout << albumName  << endl;

    //display artists from a particualr album
    for(const auto &artist: artistsSet)
    {
        cout << " " << artist << endl;
    }

    //display songs from a particular album
    for(const auto &songs: songsSet)
    {
        cout << "   " << songs << endl;
    }
}
//---------------------------------------------------------------------------------------------------------------
/// @brief the stats functions displays the overall stats of the music library and when passed with flag -d 
///         it displays overall totals and prints the current contents of the music library
/// @param musicLibrary a map of structs of type Album containing all album related information
/// @param uniqueArtists a set of strings containing all unique artists in our current library
/// @param songsInLibrary a set of strings containing all unique songs in our library 
/// @param flag a string that tells us whether to display overall totals along with tthe entirity of out music library
void stats(map<string, Album> &musicLibrary, set<string> &uniqueArtists, set<string> &songsInLibrary, string &flag)
{
    //if flag is -d display the entirity of our music library
    if(flag == "-d")
    {
        cout << "Overall Music Library Stats" << endl;
        cout << "===========================" << endl;
        cout << "Total Records: "  << musicLibrary.size()<< endl;
        cout << "Total Unique Artists: " << uniqueArtists.size() << endl;
        cout << "Total Songs: " << songsInLibrary.size() << endl << endl;

        cout << "Your Current Music Library Includes" << endl;
        cout << "===================================" << endl;

        for(auto &album: musicLibrary)
        {
            displayAlbumsObjects(album.first, album.second.artist, album.second.songs);
        }

        cout << endl;

    }else//display totals for our music library
    {
        cout << "Overall Music Library Stats" << endl;
        cout << "===========================" << endl;
        cout << "Total Records: "  << musicLibrary.size()<< endl;
        cout << "Total Unique Artists: " << uniqueArtists.size() << endl;
        cout << "Total Songs: " << songsInLibrary.size() << endl << endl;
    }

}
//---------------------------------------------------------------------------------------------------------------
/// @brief the clear function clears all contents in our map as well as all previos related 
///         information such as unique artists and songs
/// @param musicLibrary a map of structs of type Album containing all album related information
/// @param uniqueArtists a set of strings containing all unique artists in our current library
/// @param songsInLibrary a set of strings containing all unique songs in our library 
void clear(map<string, Album> &musicLibrary, set<string> &uniqueArtists, set<string> &songsInLibrary)
{
    //Clearr all data from our music library
    musicLibrary.clear();
    uniqueArtists.clear();
    songsInLibrary.clear();
}
//---------------------------------------------------------------------------------------------------------------
/// @brief the function exports the set of artist and songs in an a particular album
/// @param outFile an ofstream where we are going to export our music data to
/// @param albumName a string containg the current album we are looping through
/// @param artistsSet a set of strings containing all unique artist for one album
/// @param songsSet a set of strings containing all unique songs for one album
void exportAlbumObjects(ofstream &outFile, string albumName, set<string> &artistsSet, set<string> &songsSet)
{
    outFile << albumName  << endl;

    //export all artist data for an album
    for(auto artist: artistsSet)
    {
        outFile << artist << endl;
    }

    //export all song data for an album
    for(auto songs: songsSet)
    {
        outFile << songs << endl;
    }

    outFile << albumName  << endl;
}
//---------------------------------------------------------------------------------------------------------------
/// @brief the export function exports all of our current music library into a file. It will be exported 
///         to musicdatabase.txt by default if no file name was provided.
/// @param musicLibrary a map of structs of type Album containing all album related information
/// @param exportFilename a string specifing the desired name for the export file
void exportLibrary(map<string, Album> &musicLibrary, string &exportFilename)
{

    if(exportFilename == "")
    {
        exportFilename = "musicdatabase.txt";
    }

    ofstream outFile;

    outFile.open(exportFilename);

    //If we are able to properlly open the file export all of our current music library
    if(outFile.is_open())
    {
        //loop though our muslibrary  map
        for(auto &album: musicLibrary)
        {
            exportAlbumObjects(outFile, album.first, album.second.artist, album.second.songs);
        }
    }

    outFile.close();  

}
//---------------------------------------------------------------------------------------------------------------
/// @brief the function finds the occurence of our desired search term in our data and stores
///         the album associated with it into a set 
/// @param album a string containg the current album we are searching through
/// @param albumObjectType a struct of type Album containing either a song or artist 
/// @param searchTerm a string containg user desired search term 
/// @param tempSet a set of strings that will contain all of the albums were search term is present
void findSearchTerm(string &album, string albumObjectType, string &searchTerm, set<string> &tempSet)
{
    size_t foundIndex;
    bool termFound = false;

    tolower(albumObjectType);

    foundIndex = albumObjectType.find(searchTerm);

    if(foundIndex != string::npos)//if we found a match turn bool to true
    {
        termFound = true;
    }

    if(termFound)//if bool variable true insert album name associated with our search
    {
        tempSet.insert(album);
    }

    termFound = false;
    
}
//------------------------------------------------------------------------------------------------------------
/// @brief the function loops through a set of either songs or artist from a particualr album. 
///         It finds and stores the albums name associated with our search term in a set
/// @param albumObjectsSet an object's album set. This could be a set of songs or artist corresponding to an album
/// @param tempSet a set of strings that will contain all of the albums were search term is present
/// @param searchTerm a string containg user desired search term 
/// @param albumTitle a string containg the current album we are searching through
void loopThroghAlbum(set<string> &albumObjectsSet, set<string> &tempSet, string &searchTerm, string &albumTitle)
{
    //search through albums object type
    for(const auto &albumsObect: albumObjectsSet)
    {
        string album = albumTitle;
        findSearchTerm(album, albumsObect, searchTerm, tempSet);
    }  

}
//------------------------------------------------------------------------------------------------------------
/// @brief the function combines sets in diffrenet ways depending on their + or - modifiers.
///         It updates and calculates our resulting set from all the search terms given
/// @param compareSet a set used for copying and comparing other sets from other search terms
/// @param tempSet a set of strings that will contain all of the albums were search term is present
/// @param resultingSet a set that will contain the resulting set for all search terms
/// @param modifier a char that tells us whether we perform + or - operations on the found sets
void updateSets(set<string> &compareSet, set<string> &tempSet, set<string> &resultingSet, char &modifier)
{
    //Pick which operation to perform depending in our search this includes modifiers
    if(modifier == 'n')
    {
        set_union(compareSet.begin(), compareSet.end(), tempSet.begin(), tempSet.end(), inserter(resultingSet, resultingSet.begin()));
    }else if(modifier == '+')
    {
        set_intersection(compareSet.begin(), compareSet.end(), tempSet.begin(), tempSet.end(), inserter(resultingSet, resultingSet.begin()));
    }else if(modifier == '-')
    {
        set_difference(compareSet.begin(), compareSet.end(), tempSet.begin(), tempSet.end(), inserter(resultingSet, resultingSet.begin()));
    }
}
//------------------------------------------------------------------------------------------------------------
/// @brief using the commmand parameter it picks the correct loop to iterate, read, and compare data
///         with our search term. It decides whether to search from our albums, song, and artist info. 
/// @param musicLibrary a map of structs of type Album containing all album related information
/// @param tempSet a set of strings that will contain all of the albums were search term is present
/// @param compareSet a set used for copying and comparing other sets from other search terms
void pickObjectsLoop(map<string, Album> & musicLibrary, set<string> &tempSet, string &command, string &searchTerm)
{

    //Decide what type of object are we search for in our library
    if(command == "album")
    {
        for(const auto &albumKey: musicLibrary)
        {
            string album = albumKey.first;
            string copyOfAlbum = albumKey.first;
            findSearchTerm(album, copyOfAlbum, searchTerm, tempSet);
        }   

    }else if(command == "artist")
    {
        for(auto &albumKey: musicLibrary)
        {
            string albumTitle = albumKey.first;
            loopThroghAlbum(albumKey.second.artist, tempSet, searchTerm, albumTitle); 
        }  
    }else if(command == "song")
    {
        for(auto &albumKey: musicLibrary)
        {
            string albumTitle = albumKey.first;
            loopThroghAlbum(albumKey.second.songs, tempSet, searchTerm, albumTitle); 
        }  
    }
    

}
//------------------------------------------------------------------------------------------------------------
/// @brief gets the a set containing all albums associated with your search term depending on
///         specific modiers like + and - as well as no modifiers.
/// @param musicLibrary a map of structs of type Album containing all album related information
/// @param tempSet a set of strings that will contain all of the albums were search term is present
/// @param compareSet a set used for copying and comparing other sets from other search terms
/// @param resultingSet a set that will contain the resulting set for all search terms
/// @param searchArtist a string containg user desired search artist
void getResultingSet(map<string, Album> & musicLibrary, set<string> &tempSet, set<string> &compareSet, 
set<string> &resultingSet, string &searchTerm, string &command)
{
    resultingSet.clear();//reset resulting set for next search
    char modifier = 'n';

    //Decide modifiers for an specific search
    if(searchTerm[0] == '+')
    {
        modifier = '+';
        searchTerm = searchTerm.substr(1);
        
        pickObjectsLoop(musicLibrary, tempSet, command, searchTerm);
        
    }else if(searchTerm[0] == '-')
    {
        modifier = '-';
        searchTerm = searchTerm.substr(1);
        pickObjectsLoop(musicLibrary, tempSet, command, searchTerm);

    }else{
        
        modifier = 'n';
        pickObjectsLoop(musicLibrary, tempSet, command, searchTerm);

    }

    //combine all of our current searched and update our resulting set
    updateSets(compareSet, tempSet, resultingSet, modifier);

}
//------------------------------------------------------------------------------------------------------------
/// @brief the search function searches for desired terms and displays the albums containing such terms
///         depending of the search. User can use special search modifiers to accuratly get 
///         the desired output such as + and - modifiers.
/// @param musicLibrary a map of structs of type Album containing all album related information
/// @param userInput a string containg user desired search terms and type of search
void search(map<string, Album> & musicLibrary, string & userInput)
{

    if(userInput == "")
    {
        cout << "Error: Search terms cannot be empty." << endl;
        cout << "No results found." << endl << endl;
        return;
    }

    string searchTerms;
    string command;

    //Split the user entry 
    splitFirstWord(userInput, command, searchTerms);
    tolower(command);
    tolower(searchTerms);

    //Resulting set from all of our individual search term sets
    set<string> resultingSet;
    //Set for comparing previous finds
    set<string> compareSet;

    //Search for albums in our library
    if(command == "album")
    {
        stringstream ss(searchTerms);
        string albumToFind;

        while(ss >> albumToFind)//loop through all searched terms
        {
            //temp set to hold current finds for our word
            set<string> tempSet;

            //get results and update sets according to our searched terms
            getResultingSet(musicLibrary, tempSet, compareSet, resultingSet, albumToFind, command);

            tempSet.clear();
            compareSet.clear();
            set_union(compareSet.begin(), compareSet.end(), resultingSet.begin(), resultingSet.end(), inserter(compareSet, compareSet.begin()));
            
        }
    }

    //Search for artists in our library
    if(command == "artist")
    {
        stringstream ss(searchTerms);
        string artistToFind;

        while(ss >> artistToFind)//loop through all searched terms
        {
            //temp set to hold current finds for our word
            set<string> tempSet;

            //get results and update sets according to our searched terms
            getResultingSet(musicLibrary, tempSet, compareSet, resultingSet, artistToFind, command);

            tempSet.clear();
            compareSet.clear();
            set_union(compareSet.begin(), compareSet.end(), resultingSet.begin(), resultingSet.end(), inserter(compareSet, compareSet.begin()));
            
        }
    }

    //Search for songs in our library
    if(command == "song")
    { 
        stringstream ss(searchTerms);
        string songToFind;

        while(ss >> songToFind)//loop through all searched terms
        {
            //temp set to hold current finds for our word
            set<string> tempSet;

            //get results and update sets according to our searched terms
            getResultingSet(musicLibrary, tempSet, compareSet, resultingSet, songToFind, command);
            
            //reset
            tempSet.clear();
            compareSet.clear();
            set_union(compareSet.begin(), compareSet.end(), resultingSet.begin(), resultingSet.end(), inserter(compareSet, compareSet.begin()));

        }
    }

    //display results of our search
    cout << "Your search results exist in the following albums: " << endl;
    for(const auto &albums: resultingSet)
    {
        cout << albums << endl;
    }
    cout << endl;

}