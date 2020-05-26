
#include "stats.h"
#include "math.h"

unsigned char stats::getChannelPixel(vector<vector<long>>* vect, PNG & im, unsigned int x, unsigned int y) {
    if (vect == &sumRed) {
        return im.getPixel(x, y)->r;
    } else if (vect == &sumGreen) {
        return im.getPixel(x, y)->g; 
    } else if (vect == &sumBlue) {
        return im.getPixel(x, y)->b; 
    } else if (vect == &sumsqRed) {
        return pow(im.getPixel(x, y)->r, 2);
    } else if (vect == &sumsqGreen) {
        return pow(im.getPixel(x, y)->g, 2);
    } else if (vect == &sumsqBlue) {
        return pow(im.getPixel(x, y)->b, 2);
    } else {
        cout <<"Error, invalid vector";
        return 0;
    }
}

stats::stats(PNG & im) {
    unsigned int x;
    unsigned int y;

    unsigned int width = im.width();
    unsigned int height = im.height();

    vector<vector<vector<long>>*> sumVectors;
    sumVectors.push_back(&sumRed);
    sumVectors.push_back(&sumGreen);
    sumVectors.push_back(&sumBlue);
    sumVectors.push_back(&sumsqRed);
    sumVectors.push_back(&sumsqGreen);
    sumVectors.push_back(&sumsqBlue);

    // Set height of 2D vectors
    std::for_each(sumVectors.begin(), sumVectors.end(), [&](vector<vector<long>>* vect) { (*vect).resize(im.height()); });

    // Set width of first row of 2D vectors
    std::for_each(sumVectors.begin(), sumVectors.end(), [&](vector<vector<long>>* vect) { (*vect)[0].resize(im.width()); });

    // Initialize sum and sumsq at (0, 0)
    sumRed[0][0] = im.getPixel(0, 0)->r;
    sumGreen[0][0] = im.getPixel(0, 0)->g;
    sumBlue[0][0] = im.getPixel(0, 0)->b;

    sumsqRed[0][0] = pow(im.getPixel(0, 0)->r, 2);
    sumsqGreen[0][0] = pow(im.getPixel(0, 0)->g, 2);
    sumsqBlue[0][0] = pow(im.getPixel(0, 0)->b, 2);

    for (y = 1; y < height; y++) {
        // int it = y;
        sumRed[y].resize(width);
        sumGreen[y].resize(width);
        sumBlue[y].resize(width);

        sumsqRed[y].resize(width);
        sumsqGreen[y].resize(width);
        sumsqBlue[y].resize(width);

        sumRed[y][0] = im.getPixel(0, y)->r + sumRed[y - 1][0];
        sumGreen[y][0] = im.getPixel(0, y)->g + sumGreen[y - 1][0];
        sumBlue[y][0] = im.getPixel(0, y)->b + sumBlue[y - 1][0];

        sumsqRed[y][0] = pow(im.getPixel(0, y)->r, 2) + sumsqRed[y - 1][0];
        sumsqGreen[y][0] = pow(im.getPixel(0, y)->g, 2) + sumsqGreen[y - 1][0];
        sumsqBlue[y][0] = pow(im.getPixel(0, y)->b, 2) + sumsqBlue[y - 1][0];
    }
    
    // Initialize first row of 2D vectors
    for (x = 1; x < im.width(); x++) {
        sumRed[0][x] = im.getPixel(x, 0)->r + sumRed[0][x - 1];
        sumGreen[0][x] = im.getPixel(x, 0)->g + sumGreen[0][x - 1];
        sumBlue[0][x] = im.getPixel(x, 0)->b + sumBlue[0][x - 1];

        sumsqRed[0][x] = pow(im.getPixel(x, 0)->r, 2) + sumsqRed[0][x - 1];
        sumsqGreen[0][x] = pow(im.getPixel(x, 0)->g, 2) + sumsqGreen[0][x - 1];
        sumsqBlue[0][x] = pow(im.getPixel(x, 0)->b, 2) + sumsqBlue[0][x - 1];
    }
    
    // Iterate through every pixel in the PNG
    for (y = 1; y < im.height(); y++) {
        for (x = 1; x < im.width(); x++) {
            // Initialize the rest of the columns and rows in the 2D vectors
            sumRed[y][x] = im.getPixel(x, y)->r + sumRed[y - 1][x] + sumRed[y][x - 1] - sumRed[y - 1][x - 1];
            sumGreen[y][x] = im.getPixel(x, y)->g + sumGreen[y - 1][x] + sumGreen[y][x - 1] - sumGreen[y - 1][x - 1];
            sumBlue[y][x] = im.getPixel(x, y)->b + sumBlue[y - 1][x] + sumBlue[y][x - 1] - sumBlue[y - 1][x - 1];

            sumsqRed[y][x] = pow(im.getPixel(x, y)->r, 2) + sumsqRed[y - 1][x] + sumsqRed[y][x - 1] - sumsqRed[y - 1][x - 1];
            sumsqGreen[y][x] = pow(im.getPixel(x, y)->g, 2) + sumsqGreen[y - 1][x] + sumsqGreen[y][x - 1] - sumsqGreen[y - 1][x - 1];
            sumsqBlue[y][x] = pow(im.getPixel(x, y)->b, 2) + sumsqBlue[y - 1][x] + sumsqBlue[y][x - 1] - sumsqBlue[y - 1][x - 1];
        }
    }
}

vector<vector<long>>* stats::getVector(char channel, int sq) {
    if (sq == 0) {
        switch(channel) {
            case 'r': return &sumRed; 
            case 'g': return &sumGreen; 
            case 'b': return &sumBlue; 
            default: cout <<"Error, invalid colour channel";
                        return new vector<vector<long>>;
        }
    } else if (sq == 1) {
        switch(channel) {
            case 'r': return &sumsqRed; 
            case 'g': return &sumsqGreen; 
            case 'b': return &sumsqBlue; 
            default: cout << "Error, invalid colour channel";
                        return new vector<vector<long>>;
        }
    } else {
        cout << "Error, invalid sq parameter";
        return new vector<vector<long>>;
    }
}

long stats::getSumsHelper(char channel, int sq, pair<int,int> ul, pair<int,int> lr){
    vector<vector<long>>* vect = getVector(channel, sq);
    if(ul.first == 0 && ul.second == 0) {
        int x = (*vect).size();
        return (*vect)[lr.second][lr.first];
    } else if (ul.first == 0) {
        return (*vect)[lr.second][lr.first] - (*vect)[ul.second - 1][lr.first];
    } else if (ul.second == 0) {
        return (*vect)[lr.second][lr.first] - (*vect)[lr.second][ul.first - 1];
    } else {
        return (*vect)[lr.second][lr.first] - (*vect)[lr.second][ul.first - 1] - (*vect)[ul.second - 1][lr.first] + (*vect)[ul.second - 1][ul.first - 1];
    }
}

/* returns the sums of all pixel values across all color channels.
 * useful in computing the score of a rectangle
 * PA3 function
 * @param channel is one of r, g, or b
 * @param ul is (x,y) of the upper left corner of the rectangle 
 * @param lr is (x,y) of the lower right corner of the rectangle */
long stats::getSum(char channel, pair<int,int> ul, pair<int,int> lr){
    return getSumsHelper(channel, 0, ul, lr);
}

long stats::getSumSq(char channel, pair<int,int> ul, pair<int,int> lr){
    return getSumsHelper(channel, 1, ul, lr);
}

/* given a rectangle, return the number of pixels in the rectangle
 * @param ul is (x,y) of the upper left corner of the rectangle 
 * @param lr is (x,y) of the lower right corner of the rectangle */
long stats::rectArea(pair<int,int> ul, pair<int,int> lr) {
    int left = ul.first;
    int top = ul.second;
    int right = lr.first + 1;
    int bottom = lr.second + 1;

    return (long) ((right - left) * (bottom - top));
}

// given a rectangle, compute its sum of squared deviations from mean, over all color channels.
long stats::getScore(pair<int,int> ul, pair<int,int> lr){
    long devRed = getSumSq('r', ul, lr) - pow(getSum('r', ul, lr), 2) / rectArea(ul, lr);
    long devGreen = getSumSq('g', ul, lr) - pow(getSum('g', ul, lr), 2) / rectArea(ul, lr);
    long devBlue = getSumSq('b', ul, lr) - pow(getSum('b', ul, lr), 2) / rectArea(ul, lr);
    return devRed + devGreen + devBlue;

}
		
RGBAPixel stats::getAvg(pair<int,int> ul, pair<int,int> lr){
    int area = rectArea(ul, lr);

    int red = (int) (getSum('r', ul, lr) / area);
    int green = (int) (getSum('g', ul, lr) / area);
    int blue = (int) (getSum('b', ul, lr) / area);

    return RGBAPixel(red, green, blue);
}
