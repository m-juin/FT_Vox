#ifndef __SPLINESMANAGER_HPP__
#define __SPLINESMANAGER_HPP__

#include <unordered_map>

#include "Spline/Spline.hpp"

namespace Vox::Game::Generation
{
    class SplinesManager
    {
        public:
            SplinesManager(const std::string &splinePath);
            void LoadSplines(const std::string &splinePath);
            ~SplinesManager() {};
            std::vector<std::string> GetSplinesNames();
            std::unordered_map<std::string, float> GetSplinesWeight() const; 

            std::unordered_map<std::string, std::pair<const Spline::Spline, float>> GetSplinesCopy() const;
            std::unordered_map<std::string, std::pair<const Spline::Spline *, float>> GetSplines() const {return this->_splines;};
    
            void SetNewWeight(std::unordered_map<std::string, float> weightMap);
        private:
            std::unordered_map<std::string, std::pair<const Spline::Spline *, float>> _splines;
            /* private */
    
    };
}

#endif // __SPLINESMANAGER_HPP__