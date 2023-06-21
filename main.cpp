//
// Created by rares on 6/13/2023.
//

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>

using namespace std;

class Video {
private:
    string mVideoId;
    uint32_t mLikes, mDislikes, mViews;

protected:
    Video(const string &_id, uint32_t _likes, uint32_t _dislikes, uint32_t _views) : mVideoId(_id), mLikes(_likes),
                                                                                     mDislikes(_dislikes),
                                                                                     mViews(_views) {}

public:
    string getId() const { return mVideoId; }

    uint32_t getDislikes() const { return mDislikes; }

    uint32_t getLikes() const { return mLikes; }

    double getSimpleScore() const { return ((double) mLikes / (double) mDislikes) + 1.00 - (1.00 / mViews); }

    virtual double getAdvancedScore() const = 0;

    void print(bool simple) const {
        cout << mVideoId << ' ';
        if (simple)
            cout << fixed << setprecision(2) << getSimpleScore();
        else
            cout << fixed << setprecision(2) << getAdvancedScore();
        cout << '\n';
    }

    ~Video() = default;
};

class StudyVideo : public Video {
public:
    StudyVideo(const string &_id, uint32_t _likes, uint32_t _dislikes, uint32_t _views) : Video(_id, _likes,
                                                                                                _dislikes, _views) {}

    double getAdvancedScore() const override {
        return getSimpleScore() * 2;
    }
};

class AnimalVideo : public Video {
public:
    AnimalVideo(const string &_id, uint32_t _likes, uint32_t _dislikes, uint32_t _views) : Video(_id, _likes,
                                                                                                 _dislikes, _views) {}

    double getAdvancedScore() const override {
        return getSimpleScore() * 0.75;
    }
};

class SportsVideo : public Video {
public:
    SportsVideo(const string &_id, uint32_t _likes, uint32_t _dislikes, uint32_t _views) : Video(_id, _likes,
                                                                                                 _dislikes, _views) {}

    double getAdvancedScore() const override {
        return getSimpleScore() * 0.5;
    }
};

class VideoFactory : public Video {
private:
    VideoFactory() = default;

public:
    static Video *createVideo(const string &id, const string &type, uint32_t likes, uint32_t dislikes, uint32_t views) {
        if (type == "Studiu")
            return new StudyVideo(id, likes, dislikes, views);
        else if (type == "Animale")
            return new AnimalVideo(id, likes, dislikes, views);
        else if (type == "Sport")
            return new SportsVideo(id, likes, dislikes, views);
        else
            throw invalid_argument("Unknown video type: " + type);
    }

};

void vecsort(vector<Video *> &videos) {
    sort(videos.begin(), videos.end(), [](Video *a, Video *b) {
        if (a->getSimpleScore() == b->getSimpleScore())
            return a->getDislikes() > b->getDislikes();
        return a->getSimpleScore() < b->getSimpleScore();
    });
}

void vecsortadv(vector<Video *> &videos) {
    sort(videos.begin(), videos.end(), [](Video *a, Video *b) {
        if (a->getAdvancedScore() == b->getAdvancedScore())
            return a->getDislikes() > b->getDislikes();
        return a->getAdvancedScore() < b->getAdvancedScore();
    });
}

int main() {
    int n;
    cin >> n;

    vector<Video *> videos;

    for (int i = 1; i <= n; i++) {
        string id, type;
        uint32_t likes, dislikes, views;
        cin >> id >> type >> views >> likes >> dislikes;
        try {
            videos.push_back(VideoFactory::createVideo(id, type, likes, dislikes, views));
        } catch (invalid_argument &e) {
            cout << "EROARE";
            return 0;
        }
    }

    char c;
    cin >> c;

    if (c == 'a') {
        vecsort(videos);
        for (auto &video: videos) {
            video->print(true);
        }
    } else if (c == 'b') {
        double avg = 0;
        for (auto &video: videos) {
            avg += video->getSimpleScore();
        }
        avg /= videos.size();

        for (auto &video: videos) {
            if (video->getSimpleScore() > avg)
                video->print(true);
        }
    } else if (c == 'c') {
        string id, type;
        uint32_t likes, dislikes, views;

        while (cin >> id >> type >> views >> likes >> dislikes) {
            bool gotit = false;
            for (auto &video: videos) {
                if (video->getId() == id) {
                    video->Video::~Video();
                    video = VideoFactory::createVideo(id, type, likes, dislikes, views);
                    gotit = true;
                    break;
                }
            }
            if (!gotit)
                videos.push_back(VideoFactory::createVideo(id, type, likes, dislikes, views));
        }

        vecsort(videos);
        for (auto &video: videos) {
            video->print(true);
        }
    } else if (c == 'd') {
        vecsortadv(videos);
        for (auto &video: videos) {
            video->print(false);
        }
    }

    return 0;
}
