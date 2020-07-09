// precompile headers
#include "pch.h"
#include "Game.h"

// inlcude common namespaces for code simplification
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Core;
using namespace Windows::UI::Popups;
using namespace Windows::System;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;
using namespace Platform;


// class definition for the core framework of the app
ref class App sealed : public IFrameworkView
{
private :
	bool didCloseWindow; // change this to true when it's time to close the window
	CGame Game;

public:
	//Functions called by Windows 
	// virtual declarations
	virtual void Initialize(Windows::ApplicationModel::Core::CoreApplicationView ^appView)
	{
		//Subscribe the OnActivated function to hand the Activated "event"
		appView->Activated += ref new TypedEventHandler<CoreApplicationView ^, IActivatedEventArgs ^>(this, &App::OnActivated);

		didCloseWindow = false;

		CoreApplication::Suspending += ref new EventHandler<SuspendingEventArgs ^>(this, &App::OnSuspending);
		CoreApplication::Resuming += ref new EventHandler<Object ^>(this, &App::OnResuming);
	}

	virtual void SetWindow(CoreWindow^ Window) 
	{
		Window->PointerPressed += ref new TypedEventHandler<CoreWindow ^, PointerEventArgs ^>(this, &App::OnPointerPressed);
		Window->PointerWheelChanged += ref new TypedEventHandler<CoreWindow ^, PointerEventArgs ^>(this, &App::OnPointerWheelChanged);
		Window->KeyDown += ref new TypedEventHandler<CoreWindow ^, KeyEventArgs ^>(this, &App::OnKeyDown);
		Window->Closed += ref new TypedEventHandler<CoreWindow ^, CoreWindowEventArgs ^>(this, &App::OnClosed);

	}

	virtual void Load(String^ EntryPoint){}

	virtual void Run()
	{
		Game.Initialize();

		// Obtain a pointer to the window
		CoreWindow^ Window = CoreWindow::GetForCurrentThread();

		// Repeat this loop until window closes
		while (!didCloseWindow)
		{
			// run ProcessEvents() to dispatch events
			Window->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);

			Game.Update();
			Game.Render();
		}
	}

	virtual void Uninitialize(){}
	
	// Event that is called when the application window is ready to be activated.
	void OnActivated(CoreApplicationView ^sender, IActivatedEventArgs ^args)
	{
		// Obtain a pointer to the window
		CoreWindow^ Window = CoreWindow::GetForCurrentThread();

		Window->Activate();
	}

	//input event handlers
	void OnPointerPressed(CoreWindow^ sender, PointerEventArgs^ args)
	{
		MessageDialog Dialog("", "");
		Dialog.Content = "X: " + args->CurrentPoint->Position.X.ToString() + " " + "Y: " + args->CurrentPoint->Position.Y.ToString();
		Dialog.Title = "Mouse position";

		Dialog.ShowAsync();
	}

	void OnPointerWheelChanged(CoreWindow^ sender, PointerEventArgs^ args)
	{
		int wheel = args->CurrentPoint->Properties->MouseWheelDelta;

		MessageDialog Dialog("", "Mouse Wheel Event");
		Dialog.Content = wheel.ToString();
		Dialog.ShowAsync();
	}

	void OnKeyDown(CoreWindow^ sender, KeyEventArgs^ args)
	{
		MessageDialog Dialog("", "");

		if (args->VirtualKey == VirtualKey::W)
		{
			Dialog.Content = "Move Forward";
			Dialog.Title = "W pressed";

			Dialog.ShowAsync();
		}
		else if (args->VirtualKey == VirtualKey::A)
		{
			Dialog.Content = "Strafe Left";
			Dialog.Title = "A pressed";

			Dialog.ShowAsync();
		}
		else if (args->VirtualKey == VirtualKey::S)
		{
			Dialog.Content = "Move back";
			Dialog.Title = "S pressed";

			Dialog.ShowAsync();
		}
		else if (args->VirtualKey == VirtualKey::D)
		{
			Dialog.Content = "Strafe Right";
			Dialog.Title = "D pressed";

			Dialog.ShowAsync();
		}
	}

	void OnSuspending(Object^ sender, SuspendingEventArgs^ args)
	{

	}

	void OnResuming(Object ^sender, Object ^args)
	{

	}

	void OnClosed(CoreWindow^ sender, CoreWindowEventArgs^ args)
	{
		didCloseWindow = true;
	}
};




// the class definition that creates an instance of core framework class
ref class AppSource sealed : IFrameworkViewSource
{
public:
	virtual IFrameworkView^ CreateView()
	{
		return ref new App();
	}
};




[MTAThreadAttribute] // defines main() as a multi-threaded-apartment function
// The starting point of the app.
int main(Array<String^>^ args)
{
	CoreApplication::Run(ref new AppSource());

	return 0;
}