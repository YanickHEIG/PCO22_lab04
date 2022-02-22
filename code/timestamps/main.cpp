/******************************************************************************
  \file main.cpp
  \author Yann Thoma
  \date 15.09.2021

  This program launches several threads allowing the retrieval of timestamps
  that allow the retrieval of context changes that have occurred. A python
  script is executed with the retrieved data to visualise the behaviour of
  the execution
  ****************************************************************************/

#include <iostream>
#include <fstream>
#include <chrono>
#include <unistd.h>
#include <limits>

#include <pcosynchro/pcothread.h>

using namespace std;

chrono::high_resolution_clock::time_point start;

const size_t NB = 1000;

vector<vector<uint64_t> > vec;

/**
 * Fill container with timestamps
 *
 * Code executed from the threads to get the timestamp of passage in the loop
 * Tese timestamps will then be used to defines context switches timings
 *
 * @param vec Contains timestamps
 *
 */
void running(int /*tid*/, vector<uint64_t> &vec)
{
    for (size_t i = 0; i < NB; i++) {
        auto end = chrono::high_resolution_clock::now();
        auto diff = chrono::duration_cast<chrono::nanoseconds>(end - start);
        vec[i] = diff.count();
    }
}

/* Container used to save all the useful informations to list contexts
 * switches */
typedef struct {
    size_t from;
    size_t to;
    uint64_t timing;
} ContextSwitch;

/**
 * Create a CSV file with the context switches timestamp. This
 * file will be used to create a graphical representation of these
 *
 * @param switches Contains context switches
 *
 */
void createCSV(const vector<ContextSwitch> &switches)
{
    ofstream csvfile;
    csvfile.open("datas_timestamps_plot.csv");

    csvfile << "time" << "," << "threadID" << endl;
    csvfile << 0 << "," << switches.at(0).from << endl;
    for(const auto switches : switches) {
        csvfile << switches.timing << "," << switches.from << endl;
        csvfile << switches.timing << "," << switches.to << endl;
    }
    csvfile.close();
}

/**
 * Fills a container with structures descibing the context switches
 * that happened during the execution of the programs
 *
 * @param data Contains the useful data to find context switches
 *
 */
void analyze(vector< vector<uint64_t> > &data)
{
    vector<size_t> index(3,0);
    vector<pair<int, uint64_t> > results;
    for(size_t i = 0; i < data.size(); i++) {
        data[i][NB] = numeric_limits<uint64_t>::max();
    }

    // This loop runs through the timestamps to find and save the
    // context switches
    for(size_t i = 0; i < data.size() * NB; i++) {
        int curindex = -1;
        if (data[0][index[0]] <= data[1][index[1]]) {
            if (data[0][index[0]] <= data[2][index[2]]) {
                curindex = 0;
            }
            else {
                if (data[1][index[1]] <= data[2][index[2]]) {
                    curindex = 1;
                }
                else {
                    curindex = 2;
                }
            }
        }
        else {
            if (data[1][index[1]] <= data[2][index[2]]) {
                curindex = 1;
            }
            else {
                curindex = 2;
            }
        }
        results.push_back({curindex, data[curindex][index[curindex]]});
        if (index[curindex] < NB) {
            index[curindex] ++;
        }
    }

    vector<ContextSwitch> switches;

    int nbChange = 0;
    int current = -1;

    // Fills a vector with structures containing informations of the
    // context switches
    for (size_t i = 0; i < results.size(); i++) {
        if (results[i].first != current) {
            if (current != -1) {
                nbChange ++;
                ContextSwitch c;
                c.from = current;

                c.to = results[i].first;
                c.timing = results[i].second;
                switches.emplace_back(c);
            }
            current = results[i].first;
        }
    }

    cout << "Nb context switches: " << nbChange << endl;

    for(const auto &s : switches)
    {
        cout << "Time : " << s.timing << " . From : "
                  << s.from << " . To : " << s.to << endl;
    }
    createCSV(switches);
}


int main()
{
    vec.push_back(vector<uint64_t>(NB + 1));
    vec.push_back(vector<uint64_t>(NB + 1));
    vec.push_back(vector<uint64_t>(NB + 1));
    start = chrono::high_resolution_clock::now();
    PcoThread t0(running, 0, ref(vec[0]));
    PcoThread t1(running, 1, ref(vec[1]));
    PcoThread t2(running, 2, ref(vec[2]));
    t0.join();
    t1.join();
    t2.join();
    analyze(vec);
    // This command directly displays the graphical results
    system("./show_graphic_from_exec.py");
    return 0;
}
