/**Shaun Dobbs
 * Dr. Ray Hashemi
 * Operating Systems Assignment 2
 * Use of semaphores
 * Due: 11/28/2017
 */
#include <cstdlib>
#include <pthread.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <semaphore.h>
//Set buffer size
#define BUFFER_SIZE 3

using namespace std;

void *producer(void *);
void *consumer(void *);
string createLetter(string str);

//Create empty and full semaphores
sem_t empty, full, mutex;
//Create a buffer of size 3
string buffer[BUFFER_SIZE];

int in = 0, out = 0; //Index variables for the buffer

int main() {

    pthread_t prodID, consID;

    sem_init(&empty, 0, 3);
    sem_init(&full, 0, 0);
    sem_init(&mutex, 0, 1);

    pthread_create(&prodID, NULL, producer, NULL);
    pthread_create(&consID, NULL, consumer, NULL);

    pthread_join(prodID, NULL);
    pthread_join(consID, NULL);

    ////////////////////////////////////////////////////////////////////////////
    int valp;
    sem_getvalue(&mutex, &valp);

    //cout << valp << endl;
    ////////////////////////////////////////////////////////////////////////////

    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutex);

    pthread_exit(NULL);

    return 0;
}

//Produce items and insert them into the buffer

void *producer(void *) {
    string product;

    int cou = 0;
    
    while (cou < 200) {
        product = "";
        product = createLetter("");

        sem_wait(&empty);
        sem_wait(&mutex);

        cout << "Producer: " << product << endl;
        cout << '\n';
        buffer[in] = product;
        in = (in + 1) % BUFFER_SIZE;

        sem_post(&mutex);
        sem_post(&full);
        cou++;
    }
    pthread_exit(NULL);
}

void *consumer(void *) {
    string letter, S, T, v1, v2;
    int index = 0, n = 0, indexV1 = 0, indexV2 = 0, count = 0, ind1 = 0, ind2 = 0;
    int dist1 = 0, dist2 = 0;
    int vowelPositions[] = {0, 4, 8, 14, 20};

    int cou;
    while (cou < 200) {
        sem_wait(&full);
        sem_wait(&mutex);


        letter = buffer[out];
        index = (int) letter.at(0) - 65;

        //Determine which message is to be printed based off of the index of the letter being consumed
        if (index % 2 == 0) {

            if (index == 0 || index == 4 || index == 8 || index == 14 || index == 20) {
                //Make change since the letter is a vowel
                ind1 = (int) letter.at(0) - 65 - 1;
                ind2 = (int) letter.at(0) - 65 + 1;
            } else {
                ind1 = (int) letter.at(0) - 65;
                ind2 = (int) letter.at(0) - 65;
            }

            //Indexed such that it will go through the entire alphabet
            for (int i = ind2; i < ind2 + 26; i++) {
                if (i % 26 == vowelPositions[0] || i % 26 == vowelPositions[1] || i % 26 == vowelPositions[2] ||
                        i % 26 == vowelPositions[3] || i % 26 == vowelPositions[4]) {
                    indexV2 = i % 26;
                    break;
                }
            }

            for (int i = ind1; i > ind1 - 26; i--) {
                if (i % 26 == vowelPositions[0] || i % 26 == vowelPositions[1] || i % 26 == vowelPositions[2] ||
                        i % 26 == vowelPositions[3] || i % 26 == vowelPositions[4]) {
                    indexV1 = i % 26;
                    break;
                }
            }

            

            v2 = (char) (indexV2 + 65);

            S += v1;
            S += letter;
            S += v2;
            
            if (letter == "A") {
                v1 = 'Z';
                dist1 = 25;
            } else {
                v1 = (char) (indexV1 + 65);
                dist1 = abs(index - indexV1) - 1;
            }

            
            dist2 = abs(indexV2 - index) - 1;

            //Print to console

            cout << "C is : " << letter << endl;
            cout << "v1 is: " << v1 << endl;
            cout << "v2 is: " << v2 << endl;
            cout << "S is: " << S << endl;
            cout << "Distance from v1 to c: " << dist1 << endl;
            cout << "Distance from v2 to c: " << dist2 << endl;
            
            if(dist1 == dist2){
                cout << "The distances from c to each vowel are the same." << endl;
                cout << "Since there is no difference, T is: " << S << endl;
            } else if (dist1 > dist2){
                cout << "The larger distance from c is v1 at " << dist1 << " spaces." << endl;
                T += v1;
                T += v1;
                T += letter;
                T += v2;
                T += v1;
                cout << "T is: " << T << endl;
            } else if (dist1 < dist2){
                cout << "The larger distance from c is v2 at " << dist2 << " spaces." << endl;
                T += v2;
                T += v1; 
                T += letter;
                T += v2;
                T += v2;
                cout << "T is: " << T << endl;
            }

            //clear everything that was used so that it can be used again
            S.clear();
            T.clear();
            index = 0;
            ind1 = 0;
            ind2 = 0;

            cout << '\n';
            
        } else {
            cout << "Consumer:  " << buffer[out] << endl;
            cout << '\n';
        }
        out = (out + 1) % BUFFER_SIZE;

        sem_post(&mutex);
        sem_post(&empty);
        cou++;
    }

    pthread_exit(NULL);
}

//Function that returns a random letter. 0 based indexing

string createLetter(string str) {
    //Generate random number between 1 and 26 (represents index of letters in alphabet)
    //srand((unsigned)time(0));
    int random = rand() % 26;

    //Cast integer into appropriate alphabet letter (upper case)
    str += (char(random + 65));

    //cout << random << " " << str << endl;
    return str;
}

