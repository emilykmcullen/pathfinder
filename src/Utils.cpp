#include "Utils.h"
#include <cmath>

float NormalizedVecsDotProduct(vec2 vecA, vec2 vecB)
{
    std::cout << "dp" << std::endl;
    //Normalize the vectors 
    float lengthA = sqrt((vecA.x)*(vecA.x) + (vecA.y)*(vecA.y));
    float lengthB = sqrt((vecB.x)*(vecB.x) + (vecB.y)*(vecB.y));

    vec2 normalizedA = { (vecA.x)/lengthA, (vecA.y)/lengthA};
    vec2 normalizedB = { (vecB.x)/lengthB, (vecB.y)/lengthB};
    float dotProduct = (normalizedA.x * normalizedB.x) + (normalizedA.y * normalizedB.y);
    return dotProduct;
}