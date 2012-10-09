#include "Precompiled.h"
#include "CommandManager.h"

#include "ApplicationCommands.h"
#include "../Gui/Login/LoginComponent.h"

#include "../../AudiusLib/AudiusLib.h"

CommandManager::CommandManager(void)
{
}

CommandManager::~CommandManager(void)
{
}

void CommandManager::getAllCommands(Array <CommandID>& commands)
{
	commands.add(ApplicationCommandIDs::play);
	commands.add(ApplicationCommandIDs::pause);
	commands.add(ApplicationCommandIDs::next);
	commands.add(ApplicationCommandIDs::previous);
	commands.add(ApplicationCommandIDs::toggleplaypause);
	commands.add(ApplicationCommandIDs::refreshplaylist);
}

void CommandManager::getCommandInfo(CommandID commandID, ApplicationCommandInfo& result)
{
	Player::Status status = AudioPlayer::getInstance()->getPlayerStatus();
	switch(commandID)
	{
	case ApplicationCommandIDs::play:
		result.setInfo("Play", "Start playing music", "Player", status == Player::Playing ? ApplicationCommandInfo::isDisabled : 0);
		break;
	case ApplicationCommandIDs::pause:
		result.setInfo("Pause", "Pause music", "Player", status != Player::Playing ? ApplicationCommandInfo::isDisabled : 0);
		result.addDefaultKeypress(KeyPress::stopKey, ModifierKeys());
		break;
	case ApplicationCommandIDs::next:
		result.setInfo("Next", "Go to next song", "Player", 0);
		result.addDefaultKeypress(KeyPress::fastForwardKey, ModifierKeys());
		break;
	case ApplicationCommandIDs::previous:
		result.setInfo("Previous", "Go to previous song", "Player", 0);
		result.addDefaultKeypress(KeyPress::rewindKey, ModifierKeys());
		break;
	case ApplicationCommandIDs::toggleplaypause:
		result.setInfo("Play/pause", "Play or pause music", "Player", 0);
		result.addDefaultKeypress(KeyPress::playKey, ModifierKeys());
		break;
	case ApplicationCommandIDs::refreshplaylist:
		result.setInfo("Refresh playlist", "Refreshes the playlist from the server", "Player", 0);
		break;
	}
}

bool CommandManager::perform(const InvocationInfo& info)
{
	AudioPlayer* player = AudioPlayer::getInstance();
	switch(info.commandID)
	{
	case ApplicationCommandIDs::play:
		if(!checkLogin())
			return true;
		player->startPlaying();
		return true;
	case ApplicationCommandIDs::pause:
		player->pausePlaying();
		return true;
	case ApplicationCommandIDs::next:
		player->goToNext();
		return true;
	case ApplicationCommandIDs::previous:
		player->goToPrevious();
		return true;
	case ApplicationCommandIDs::toggleplaypause:
		if(player->getPlayerStatus() != Player::Playing && !checkLogin())
			return true;
		if(player->getPlaylist()->getCount() == 0)
			player->refreshPlaylist();

		player->togglePlayPause();
		return true;
	case ApplicationCommandIDs::refreshplaylist:
		if(!AlertWindow::showOkCancelBox(
			AlertWindow::QuestionIcon,
			"Refresh playlist",
			"Are you sure you want to retrieve the playlist from the server?\n(This will empty your current playlist.)"))
			return true;

		player->refreshPlaylist();
		AlertWindow::showMessageBox(AlertWindow::InfoIcon, "Refresh playlist", "Your playlist has been refreshed from the server.");
		return true;
	}
	return false;
}

bool CommandManager::checkLogin()
{
	if(ServiceManager::getInstance()->isLoggedIn())
		return true;

	// Show login dialog
	LoginComponent lc;
	int result = DialogWindow::showModalDialog("Clodder Login", &lc, NULL, Colours::azure, true);
	if(result == 0)
		return false;	// User canceled

	if(!ServiceManager::getInstance()->getClodder()->login(lc.getLogin(), lc.getPassword()))
	{
		// Login failed
		Log::write("Could not start playing. Login failed.");
		AlertWindow::showMessageBox(AlertWindow::WarningIcon, "Login failed.", "Could not start playing. Login failed.");
		return false;
	}

	return true;
}

ApplicationCommandTarget* CommandManager::getNextCommandTarget()
{
	return NULL;
}
