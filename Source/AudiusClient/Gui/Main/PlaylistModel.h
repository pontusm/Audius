#pragma once
////////////////////////////////////////////////////////////////////////////////////////
//	   File: PlaylistModel.h
////////////////////////////////////////////////////////////////////////////////////////
// Description:
//
// Represents the data model for the playlist.
//
////////////////////////////////////////////////////////////////////////////////////////

#include "juce.h"

class MusicPlayer;

class PlaylistModel : public TableListBoxModel
{
public:
	PlaylistModel(MusicPlayer* player);
	virtual ~PlaylistModel(void);

	virtual int getNumRows();
	virtual void paintRowBackground(Graphics& g, int rowNumber, int width, int height, bool rowIsSelected);
	virtual void paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected);

private:
	MusicPlayer* _player;
};
