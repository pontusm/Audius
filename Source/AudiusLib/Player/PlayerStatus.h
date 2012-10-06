#pragma once
////////////////////////////////////////////////////////////////////////////////////////
//	   File: PlayerStatus.h
////////////////////////////////////////////////////////////////////////////////////////
// Description:
//
// Contains the possible statuses of the music player.
//
////////////////////////////////////////////////////////////////////////////////////////

namespace Player
{
	enum Status
	{
		Unknown,
		Stopped,
		Paused,
		//Buffering,
		Playing
	};

}

namespace PlayerNotifications
{
	static const String newSong					= "newsong";
	static const String songInfoChanged			= "songinfochg";
	static const String statusChanged			= "statuschg";
	static const String playlistChanged			= "playlistchg";
	static const String serverPlaylistUpdated	= "serverplaylistupdated";
}
