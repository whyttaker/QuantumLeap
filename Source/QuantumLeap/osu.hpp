#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>

namespace osu
{
    std::vector<std::string> split(std::string s, std::string delim)
    {
        std::vector<std::string> res;
        size_t pos = 0;
        std::string token;
        while ((pos = s.find(delim)) != std::string::npos)
        {
            token = s.substr(0, pos);
            res.push_back(token);
            s.erase(0, pos + delim.length());
        }
        res.push_back(s);
        return res;
    }

#pragma region HitObject
    enum class HitObjectType
    {
        CIRCLE = 0,
        SLIDER = 1,
        SPINNER = 3
    };

    class HitObject
    {
    public:
        HitObject(int x, int y, HitObjectType type, int time, int length)
        {
            this->x = x;
            this->y = y;
            this->type = type;
            this->time = time;
            this->length = length;
        }

    private:
        int x;
        int y;
        HitObjectType type;
        int time;
        int length;

    public:
        int X() const
        {
            return x;
        }

        int Y() const
        {
            return y;
        }

        HitObjectType Type() const
        {
            return type;
        }

        int Time() const
        {
            return time;
        }

        int Length() const
        {
            return length;
        }

        std::string toString() const
        {
            std::string typeStr = "";
            switch (type)
            {
            case HitObjectType::CIRCLE:
                typeStr = "CIRCLE";
                break;
            case HitObjectType::SLIDER:
                typeStr = "SLIDER";
                break;
            case HitObjectType::SPINNER:
                typeStr = "SPINNER";
                break;
            }

            return "Type: " + typeStr + ", Time: " + std::to_string(time) + ", Length: " + std::to_string(length);
        }
    };

    std::ostream& operator<<(std::ostream& os, const HitObject& obj)
    {
        os << obj.toString();
        return os;
    }
#pragma endregion HitObject

#pragma region TimingPoint
    class TimingPoint
    {
    public:
        TimingPoint(int time, float beatLength, bool uninherited)
        {
            this->time = time;
            this->beatLength = beatLength;
            this->uninherited = uninherited;
        }

    private:
        int time;
        float beatLength;
        bool uninherited;

    public:
        int Time() const
        {
            return time;
        }

        float BeatLength() const
        {
            return beatLength;
        }

        bool Uninherited() const
        {
            return uninherited;
        }

        float SliderVelocity() const
        {
            if (uninherited)
            {
                return -100.0f / beatLength;
            }
            else
            {
                return 1.0f;
            }
        }
    };

#pragma region Beatmap
    class Beatmap
    {
    public:
        Beatmap(std::string osufile)
        {
            leadin = 0;
            processOsuFile(osufile);
        }

    private:
        int leadin;
        std::vector<HitObject> hitObjects;
        std::string audioFilename;
        std::string title;
        std::string artist;
        int beatmapID;

    public:
        int LeadIn() const
        {
            return leadin;
        }

        int EndTime() const
        {
            const HitObject& last = hitObjects.back();
            return last.Time() + last.Length();
        }

        const std::vector<HitObject>&
            HitObjects() const
        {
            return hitObjects;
        }

        const std::string& AudioFilename() const
        {
            return audioFilename;
        }

        const std::string& Title() const
        {
            return title;
        }

        const std::string& Artist() const
        {
            return artist;
        }

        int BeatmapID() const
        {
            return beatmapID;
        }

    private:
        void processOsuFile(std::string osufile)
        {
            std::stringstream osu(osufile);

            std::vector<TimingPoint> timingPoints;

            const auto GeneralFunc = [&](std::string line)
            {
                if (line.rfind("AudioFilename") == 0)
                {
                    audioFilename = line.substr(line.find(':') + 1);
                }
                else if (line.rfind("AudioLeadIn") == 0)
                {
                    leadin = stoi(line.substr(line.find(':') + 1));
                }
            };

            const auto MetadataFunc = [&](std::string line)
            {
                if (line.rfind("Title") == 0)
                {
                    title = line.substr(line.find(':') + 1);
                }
                else if (line.rfind("Artist") == 0)
                {
                    artist = line.substr(line.find(':') + 1);
                }
                else if (line.rfind("BeatmapID") == 0)
                {
                    beatmapID = stoi(line.substr(line.find(':') + 1));
                }
            };

            float sliderMultiplier = 1.0f;
            const auto DifficultyFunc = [&](std::string line)
            {
                if (line.rfind("SliderMultiplier") == 0)
                {
                    sliderMultiplier = stof(line.substr(line.find(':') + 1));
                }
            };

            const auto TimingPointFunc = [&](std::string line)
            {
                std::vector<std::string> args = split(line, ",");

                int time = stoi(args[0]) + leadin;
                float beatLength = stof(args[1]);
                int uninherited = stoi(args[6]);

                timingPoints.emplace_back(time, beatLength, uninherited == 0);
            };

            const auto HitObjFunc = [&](std::string line)
            {
                std::vector<std::string> args = split(line, ",");

                int x = stoi(args[0]);
                int y = stoi(args[1]);

                int time = stoi(args[2]) + leadin;
                int noteLength = 0;

                float sliderVelocity = 1.0f;
                float beatLength = timingPoints[0].BeatLength();

                for (int i = 0; i < timingPoints.size(); i++)
                {
                    if (timingPoints[i].Time() <= time)
                    {
                        if (!timingPoints[i].Uninherited())
                        {
                            sliderVelocity = timingPoints[i].SliderVelocity();
                        }
                    }
                    else
                    {
                        break;
                    }
                }

                int type = stoi(args[3]);
                HitObjectType hitType = HitObjectType::CIRCLE;
                if (type & 0b00000010)
                {
                    hitType = HitObjectType::SLIDER;
                    int length = stoi(args[7]);
                    int slides = stoi(args[6]);
                    noteLength = int(length / (sliderMultiplier * 100 * sliderVelocity) * beatLength);
                }
                else if (type & 0b00001000)
                {
                    hitType = HitObjectType::SPINNER;
                    noteLength = stoi(args[5]) - time;
                }

                hitObjects.emplace_back(x, y, hitType, time, noteLength);
            };

            std::string line = "";
            std::string section = "";
            while (getline(osu, line))
            {
                if (line.length() < 1)
                    continue;

                if (line[0] == '[')
                {
                    section = line.substr(1, line.find_last_of(']') - 1);
                }
                else if (section == "General")
                {
                    GeneralFunc(line);
                }
                else if (section == "Metadata")
                {
                    MetadataFunc(line);
                }
                else if (section == "Difficulty")
                {
                    DifficultyFunc(line);
                }
                else if (section == "TimingPoints")
                {
                    TimingPointFunc(line);
                }
                else if (section == "HitObjects")
                {
                    HitObjFunc(line);
                }
            }
        }
    };

    std::ostream& operator<<(std::ostream& os, const Beatmap& song)
    {
        os << "Title: " << song.Title() << std::endl;
        os << "Artist: " << song.Artist() << std::endl;
        os << "AudioFilename: " << song.AudioFilename() << std::endl;
        os << "BeatmapID: " << song.BeatmapID() << std::endl;
        os << "LeadIn: " << song.LeadIn() << "ms" << std::endl;

        int numCircles = 0;
        int numSliders = 0;
        int numSpinners = 0;
        for (const HitObject& obj : song.HitObjects())
        {
            switch (obj.Type())
            {
            case HitObjectType::CIRCLE:
                numCircles++;
                break;
            case HitObjectType::SLIDER:
                numSliders++;
                break;
            case HitObjectType::SPINNER:
                numSpinners++;
                break;
            }
        }
        os << song.HitObjects().size() << " HitObjects (" << numCircles << " circles, " << numSliders << " sliders, " << numSpinners << " spinners)" << std::endl;

        return os;
    }
#pragma endregion Beatmap
}
