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
	static const String newSong					= T("newsong");
	static const String songInfoChanged			= T("songinfochg");
	static const String statusChanged			= T("statuschg");
	static const String playlistChanged			= T("playlistchg");
	static const String serverPlaylistUpdated	= T("serverplaylistupdated");
}
