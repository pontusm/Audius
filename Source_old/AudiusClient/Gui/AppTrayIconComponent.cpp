#include "Precompiled.h"
#include "AppTrayIconComponent.h"

#include "../Core/ApplicationCommands.h"

#include "../Resources/ResourceFiles.h"

AppTrayIconComponent::AppTrayIconComponent( ApplicationCommandManager* commandManager ) :
	_commandManager(commandManager),
	_icon(0)
{
	_icon = ImageFileFormat::loadFrom(ResourceFiles::eket_mask_png, ResourceFiles::eket_mask_pngSize);
	if(_icon.isValid())
		setIconImage(_icon);

	setIconTooltip ("Audius music player");
}

AppTrayIconComponent::~AppTrayIconComponent()
{
}

void AppTrayIconComponent::mouseDown( const MouseEvent& e )
{
	PopupMenu m;
	m.addCommandItem(_commandManager, ApplicationCommandIDs::play);
	m.addCommandItem(_commandManager, ApplicationCommandIDs::pause);
	m.addCommandItem(_commandManager, ApplicationCommandIDs::next);
	m.addCommandItem(_commandManager, ApplicationCommandIDs::previous);
	m.addSeparator();
	m.addCommandItem(_commandManager, ApplicationCommandIDs::refreshplaylist);
	m.addSeparator();
	//m.addItem(99, T("Quit"));
	m.addCommandItem(_commandManager, StandardApplicationCommandIDs::quit);

	//const int result = m.show();
	// Note: Commands are automatically passed to the app command manager
	m.show();
	//if (result == 99)
	//	JUCEApplication::getInstance()->systemRequestedQuit();
	//if(result > 0)
	//	_commandManager->invokeDirectly(result, false);
}

