//==================================================================================================
/**
  MudPP - MUD engine for C++
  Copyright 2019-2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef MUDPP_ENGINE_GAME_HPP_INCLUDED
#define MUDPP_ENGINE_GAME_HPP_INCLUDED

#include <mudpp/system/session_manager.hpp>
#include <mudpp/engine/player.hpp>
#include <boost/asio.hpp>
#include <sol/sol.hpp>
#include <vector>
#include <map>

namespace mudpp
{
  struct session;

  struct game
  {
    game( std::string const& config_file );

    std::ostream& log(std::ostream& os, std::string const& context);

    bool run();
    void shutdown();


    player* find_player(std::string const& name);
    player* attach_player(session&);

    std::map<std::string,std::string>&  strings()       { return messages_.value();   }
    sol::state&                         script_engine() { return lua_state_;  }
    boost::asio::io_service&            io()            { return ios_;        }
    std::map<std::string,std::string>&  paths()         { return paths_.value(); }

    bool exists( player const& p);

    template<typename Function> void register_event(int period, Function f)
    {
      events_.push_back( periodic_event::make(ios_, period, f) );
    }

    private:

    void lua_setup();
    void cleanup();

    boost::asio::io_service                         ios_;
    std::vector<periodic_event_t>                   events_;
    std::vector<player_t>                           players_;
    sol::nested<std::map<std::string, std::string>> paths_;
    sol::nested<std::map<std::string, std::string>> messages_;
    std::unique_ptr<mudpp::session_manager>         sessions_;
    sol::state                                      lua_state_;
    int                                             period_;
    bool                                            shutdown_;
  };

}

#endif
