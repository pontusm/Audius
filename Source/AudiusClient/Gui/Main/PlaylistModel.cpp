#include "Precompiled.h"
#include "PlaylistModel.h"

#include "../../../AudiusLib/AudiusLib.h"

PlaylistModel::PlaylistModel(MusicPlayer* player) :
	_player(player)
{
}

PlaylistModel::~PlaylistModel(void)
{
}

int PlaylistModel::getNumRows()
{
	return _player->getPlaylistCount();
}

void PlaylistModel::paintRowBackground( Graphics& g, int rowNumber, int width, int height, bool rowIsSelected )
{
	g.fillAll(Colours::red);
}

void PlaylistModel::paintCell( Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected )
{

}