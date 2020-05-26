#define CATCH_CONFIG_MAIN
#include "cs221util/catch.hpp"
#include <vector>
#include <sys/stat.h>
#include <iostream>
#include "cs221util/PNG.h"
#include "cs221util/RGBAPixel.h"
#include "stats.h"
#include "twoDtree.h"

using namespace std;
using namespace cs221util;


TEST_CASE("stats::basic rectArea","[weight=1][part=stats]"){


    PNG data; data.resize(2,3);

    stats s(data);
    pair<int,int> ul(0,0);
    pair<int,int> lr(1,2);
    long result = s.rectArea(ul,lr);

    REQUIRE(result == 6);
    
}

TEST_CASE("stats::basic getAvg","[weight=1][part=stats]"){
    PNG data; data.resize(2,2);
    for (int i = 0; i < 2; i ++){
        for (int j = 0; j < 2; j++){
            RGBAPixel * p = data.getPixel(i,j);
            p->r = 20 * i + 3 * j;
            p->g = 3 * i + 20 * j;
            p->b = 23 * i + 23 * j;
            p->a = 1.0;
        }
    }
    stats s(data);
    pair<int,int> ul(0,0);
    pair<int,int> lr(1,1);
    RGBAPixel result = s.getAvg(ul,lr);
    RGBAPixel expected(11,11,23);

    REQUIRE(result == expected);
}

TEST_CASE("stats::basic 1x2 getAvg (0,0)(1,0)","[weight=1][part=stats]"){
    PNG data; data.resize(2,1);
    for (int i = 0; i < 2; i ++){
        for (int j = 0; j < 1; j++){
            RGBAPixel * p = data.getPixel(i,j);
            p->r = 12;
            p->g = 12;
            p->b = 12;
            p->a = 1.0;
        }
    }
    stats s(data);
    pair<int,int> ul(0,0);
    pair<int,int> lr(1,0);
    cout << "area" << s.rectArea(ul, lr);
    RGBAPixel result = s.getAvg(ul,lr);
    cout << result.r << " " << result.g << " " << result.b;
    RGBAPixel expected(13,13,13);

    REQUIRE(result == expected);
}

TEST_CASE("stats::size 1x2,(1,0)(1,0) avg","[weight=1][part=stats]"){
    PNG data; data.resize(2,1);
    for (int i = 0; i < 2; i ++){
        for (int j = 0; j < 1; j++){
            RGBAPixel * p = data.getPixel(i,j);
            p->r = 12;
            p->g = 12;
            p->b = 12;
            p->a = 1.0;
        }
    }
    stats s(data);
    pair<int,int> ul(1,0);
    pair<int,int> lr(1,0);
    cout << "area" << s.rectArea(ul, lr);
    RGBAPixel result = s.getAvg(ul,lr);
    cout << result.r << " " << result.g << " " << result.b;
    RGBAPixel expected(13,13,13);

    REQUIRE(result == expected);
}


TEST_CASE("stats::basic variance","[weight=1][part=stats]"){
    PNG data; data.resize(2,2);
    for (int i = 0; i < 2; i ++){
        for (int j = 0; j < 2; j++){
            RGBAPixel * p = data.getPixel(i,j);
            p->r = 20 * i + 3 * j;
            p->g = 3 * i + 20 * j;
            p->b = 23 * i + 23 * j;
            p->a = 1.0;
        }
    }
    stats s(data);
    pair<int,int> ul(0,0);
    pair<int,int> lr(1,1);
    long result = s.getScore(ul,lr);

    REQUIRE(result == 1876);
}

TEST_CASE("stats::2x2 score","[weight=1][part=stats]"){
    PNG data; data.resize(2,2);
    for (int i = 0; i < 2; i ++){
        for (int j = 0; j < 2; j++){
            RGBAPixel * p = data.getPixel(i,j);
            p->r = 64;
            p->g = 64;
            p->b = 64;
            p->a = 1.0;
        }
    }
    stats s(data);
    pair<int,int> ul(0,0);
    pair<int,int> lr(1,1);
    long result = s.getScore(ul,lr);

    REQUIRE(result == 0);
}

TEST_CASE("twoDtree::basic ctor render","[weight=1][part=twoDtree]"){
    PNG img;
    img.readFromFile("images/geo.png");

    twoDtree t1(img);

    PNG out = t1.render();

    out.writeToFile("images/rendertest_geo.png");

    REQUIRE(out==img);
}


TEST_CASE("twoDtree::basic copy","[weight=1][part=twoDtree]"){
    PNG img;
    img.readFromFile("images/geo.png");

    twoDtree t1(img);
    twoDtree t1copy(t1);

    PNG out = t1copy.render();

    REQUIRE(out==img);
}

// TEST_CASE("twoDtree::basic prune canada","[weight=1][part=twoDtree]"){
//     PNG img;
//     img.readFromFile("images/canadaPlace.png");
    
//     twoDtree t1(img); 
//     t1.prune(4000);
//     PNG result = t1.render();
//     result.writeToFile("images/out_canadaPlacePrune.png");
// }

// TEST_CASE("twoDtree::basic prune","[weight=1][part=twoDtree]"){
//     PNG img;
//     img.readFromFile("images/ada.png");
    
//     twoDtree t1(img); 
//     t1.prune(3000);
//     PNG result = t1.render();
//     result.writeToFile("images/out_adaPrune.png");

//     PNG expected; 
//     expected.readFromFile("images/given-adaPrune.png");

//     REQUIRE(expected==result);
// }

TEST_CASE("twoDtree::basic pruneSize","[weight=1][part=twoDtree]"){
    PNG img;
    img.readFromFile("images/ada.png");
    
    twoDtree t1(img); 
    int result = t1.pruneSize(3000);

    int expected = 13904;

    REQUIRE(expected==result);
}

TEST_CASE("twoDtree::basic idealPrune","[weight=1][part=twoDtree]"){
    PNG img;
    img.readFromFile("images/ada.png");
    
    twoDtree t1(img); 
    int result = t1.idealPrune(13904);

    int expected = 2998;

    REQUIRE(expected==result);
}
