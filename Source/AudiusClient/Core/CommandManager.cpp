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
		result.setInfo(T("Play"), T("Start playing music"), T("Player"), status == Player::Playing ? ApplicationCommandInfo::isDisabled : 0);
		break;
	case ApplicationCommandIDs::pause:
		result.setInfo(T("Pause"), T("Pause music"), T("Player"), status != Player::Playing ? ApplicationCommandInfo::isDisabled : 0);
		result.addDefaultKeypress(KeyPress::stopKey, ModifierKeys());
		break;
	case ApplicationCommandIDs::next:
		result.setInfo(T("Next"), T("Go to next song"), T("Player"), 0);
		result.addDefaultKeypress(KeyPress::fastForwardKey, ModifierKeys());
		break;
	case ApplicationCommandIDs::previous:
		result.setInfo(T("Previous"), T("Go to previous song"), T("Player"), 0);
		result.addDefaultKeypress(KeyPress::rewindKey, ModifierKeys());
		break;
	case ApplicationCommandIDs::toggleplaypause:
		result.setInfo(T("Play/pause"), T("Play or pause music"), T("Player"), 0);
		result.addDefaultKeypress(KeyPress::playKey, ModifierKeys());
		break;
	case ApplicationCommandIDs::refreshplaylist:
		result.setInfo(T("Refresh playlist"), T("Refreshes the playlist from the server"), T("Player"), 0);
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
			T("Refresh playlist"),
			T("Are you sure you want to retrieve the playlist from the server?\n(This will empty your current playlist.)")))
			return true;

		player->refreshPlaylist();
		AlertWindow::showMessageBox(AlertWindow::InfoIcon, T("Refresh playlist"), T("Your playlist has been refreshed from the server."));
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
	int result = DialogWindow::showModalDialog(T("Clodder Login"), &lc, NULL, Colours::azure, true);
	if(result == 0)
		return false;	// User canceled

	if(!ServiceManager::getInstance()->getClodder()->login(lc.getLogin(), lc.getPassword()))
	{
		// Login failed
		Log::write("Could not start playing. Login failed.");
		AlertWindow::showMessageBox(AlertWindow::WarningIcon, T("Login failed."), T("Could not start playing. Login failed."));
		return false;
	}

	return true;
}

ApplicationCommandTarget* CommandManager::getNextCommandTarget()
{
	return NULL;
}
