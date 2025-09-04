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

            std::unordered_map<std::string, const Spline::Spline> GetSplinesCopy() const;
    
        private:
            std::unordered_map<std::string, const Spline::Spline *> _splines;
            /* private */
    
    };
}

#endif // __SPLINESMANAGER_HPP__