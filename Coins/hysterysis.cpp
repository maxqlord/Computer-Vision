//
// Created by Max Lord on 1/16/18.
//


/*
 *
    // Step 4: Hysterysis threshold
    /*bool change = true; //if image changed- will be set to true if new edge pixels found
    int counter = 0; //counter
    while (change) {
        change = false;
        //printf("Iteration %d", i);
        counter++;
        //initialize iterators
        sobelIterator = sum.begin<float>();
        angleIterator = angle.begin<double>();
        finalIterator = finalImg.begin<unsigned char>();
        endIterator = sum.end<float>();
        for (; sobelIterator != endIterator; ++sobelIterator, ++angleIterator, ++finalIterator) {
            //get pixel gradient direction and position
            cv::Point p = finalIterator.pos();
            if (p.x < 2 || p.x >= src.cols - 2 || p.y < 2 || p.y >= src.rows - 2)
                continue;
            float currentangle = *angleIterator;

            // Do we have a pixel we already know as an edge?
            if (*finalIterator == 255) {
                //pixel already known to be an edge
                *finalIterator = (unsigned char) 64;



                The condition used in nonmaximum suppression still holds
                For (x-1, y-1) this means checking against (x, y-2) and (x-2, y)
                For (x+1, y+1) this means checking against the (x, y+2) and (x+2, y)
                 check if pixels (x-1, y-1) and (x+1, y+1) are edge pixels
                if (currentangle > 112.5 && currentangle <= 157.5) {
                    if (p.y > 0 && p.x > 0) {
                        //The gradient magnitude at these points is greater than the lower threshold
                        if (lower <= sum.at<float>(p.y - 1, p.x - 1) &&
                            finalImg.at<unsigned char>(p.y - 1, p.x - 1) != 64 && //hasn't already been checked
                            angle.at<float>(p.y - 1, p.x - 1) > 112.5 &&  //check if direction at pixel is same direction as currentangle
                            angle.at<float>(p.y - 1, p.x - 1) <= 157.5 &&
                            sum.at<float>(p.y - 1, p.x - 1) > sum.at<float>(p.y - 2, p.x) &&  //For (x-1, y-1) check against (x, y-2) and (x-2, y)
                            sum.at<float>(p.y - 1, p.x - 1) > sum.at<float>(p.y, p.x - 2)) {  // For (x+1, y+1) check against the (x, y+2) and (x+2, y)
                            finalImg.ptr<unsigned char>(p.y - 1, p.x - 1)[0] = 255;
                            change = true;
                        }
                    }
                    if (p.y < src.rows - 1 && p.x < src.cols - 1) {
                        if (lower <= sum.at<float>(cv::Point(p.x + 1, p.y + 1)) &&
                            finalImg.at<unsigned char>(p.y + 1, p.x + 1) != 64 &&
                            angle.at<float>(p.y + 1, p.x + 1) > 112.5 &&
                            angle.at<float>(p.y + 1, p.x + 1) <= 157.5 &&
                            sum.at<float>(p.y - 1, p.x - 1) > sum.at<float>(p.y + 2, p.x) &&
                            sum.at<float>(p.y - 1, p.x - 1) > sum.at<float>(p.y, p.x + 2)) {
                            finalImg.ptr<unsigned char>(p.y + 1, p.x + 1)[0] = 255;
                            change = true;
                        }
                    }
                } else if (currentangle > 67.5 && currentangle <= 112.5) {
                    if (p.x > 0) {
                        if (lower <= sum.at<float>(cv::Point(p.x - 1, p.y)) &&
                            finalImg.at<unsigned char>(p.y, p.x - 1) != 64 &&
                            angle.at<float>(p.y, p.x - 1) > 67.5 &&
                            angle.at<float>(p.y, p.x - 1) <= 112.5 &&
                            sum.at<float>(p.y, p.x - 1) > sum.at<float>(p.y - 1, p.x - 1) &&
                            sum.at<float>(p.y, p.x - 1) > sum.at<float>(p.y + 1, p.x - 1)) {
                            finalImg.ptr<unsigned char>(p.y, p.x - 1)[0] = 255;
                            change = true;
                        }
                    }
                    if (p.x < src.cols - 1) {
                        if (lower <= sum.at<float>(cv::Point(p.x + 1, p.y)) &&
                            finalImg.at<unsigned char>(p.y, p.x + 1) != 64 &&
                            angle.at<float>(p.y, p.x + 1) > 67.5 &&
                            angle.at<float>(p.y, p.x + 1) <= 112.5 &&
                            sum.at<float>(p.y, p.x + 1) > sum.at<float>(p.y - 1, p.x + 1) &&
                            sum.at<float>(p.y, p.x + 1) > sum.at<float>(p.y + 1, p.x + 1)) {
                            finalImg.ptr<unsigned char>(p.y, p.x + 1)[0] = 255;
                            change = true;
                        }
                    }
                } else if (currentangle > 22.5 && currentangle <= 67.5) {
                    if (p.y > 0 && p.x < src.cols - 1) {
                        if (lower <= sum.at<float>(cv::Point(p.x + 1, p.y - 1)) &&
                            finalImg.at<unsigned char>(p.y - 1, p.x + 1) != 64 &&
                            angle.at<float>(p.y - 1, p.x + 1) > 22.5 &&
                            angle.at<float>(p.y - 1, p.x + 1) <= 67.5 &&
                            sum.at<float>(p.y - 1, p.x + 1) > sum.at<float>(p.y - 2, p.x) &&
                            sum.at<float>(p.y - 1, p.x + 1) > sum.at<float>(p.y, p.x + 2)) {
                            finalImg.ptr<unsigned char>(p.y - 1, p.x + 1)[0] = 255;
                            change = true;
                        }
                    }
                    if (p.y < src.rows - 1 && p.x > 0) {
                        if (lower <= sum.at<float>(cv::Point(p.x - 1, p.y + 1)) &&
                            finalImg.at<unsigned char>(p.y + 1, p.x - 1) != 64 &&
                            angle.at<float>(p.y + 1, p.x - 1) > 22.5 &&
                            angle.at<float>(p.y + 1, p.x - 1) <= 67.5 &&
                            sum.at<float>(p.y + 1, p.x - 1) > sum.at<float>(p.y, p.x - 2) &&
                            sum.at<float>(p.y + 1, p.x - 1) > sum.at<float>(p.y + 2, p.x)) {
                            finalImg.ptr<unsigned char>(p.y + 1, p.x - 1)[0] = 255;
                            change = true;
                        }
                    }
                } else {
                    if (p.y > 0) {
                        if (lower <= sum.at<float>(cv::Point(p.x, p.y - 1)) &&
                            finalImg.at<unsigned char>(p.y - 1, p.x) != 64 &&
                            (angle.at<float>(p.y - 1, p.x) < 22.5 ||
                             angle.at<float>(p.y - 1, p.x) >= 157.5) &&
                            sum.at<float>(p.y - 1, p.x) > sum.at<float>(p.y - 1, p.x - 1) &&
                            sum.at<float>(p.y - 1, p.x) > sum.at<float>(p.y - 1, p.x + 2)) {
                            finalImg.ptr<unsigned char>(p.y - 1, p.x)[0] = 255;
                            change = true;
                        }
                    }
                    if (p.y < src.rows - 1) {
                        if (lower <= sum.at<float>(cv::Point(p.x, p.y + 1)) &&
                            finalImg.at<unsigned char>(p.y + 1, p.x) != 64 &&
                            (angle.at<float>(p.y + 1, p.x) < 22.5 ||
                             angle.at<float>(p.y + 1, p.x) >= 157.5) &&
                            sum.at<float>(p.y + 1, p.x) > sum.at<float>(p.y + 1, p.x - 1) &&
                            sum.at<float>(p.y + 1, p.x) > sum.at<float>(p.y + 1, p.x + 1)) {
                            finalImg.ptr<unsigned char>(p.y + 1, p.x)[0] = 255;
                            change = true;
                        }
                    }
                }
            }
        }
    }*/

