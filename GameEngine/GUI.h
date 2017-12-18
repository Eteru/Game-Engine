#pragma once

#include <iostream>

#include <nanogui/screen.h>
#include <nanogui/window.h>
#include <nanogui/layout.h>
#include <nanogui/label.h>
#include <nanogui/checkbox.h>
#include <nanogui/button.h>
#include <nanogui/toolbutton.h>
#include <nanogui/popupbutton.h>
#include <nanogui/combobox.h>
#include <nanogui/progressbar.h>
#include <nanogui/entypo.h>
#include <nanogui/messagedialog.h>
#include <nanogui/textbox.h>
#include <nanogui/slider.h>
#include <nanogui/imagepanel.h>
#include <nanogui/imageview.h>
#include <nanogui/vscrollpanel.h>
#include <nanogui/colorwheel.h>
#include <nanogui/graph.h>
#include <nanogui/formhelper.h>
#include <direct.h>


using namespace nanogui;
using std::cout;
using std::cerr;
using std::endl;

class GUI : public nanogui::Screen
{
public:
	GUI(SDL_Window* pwindow, int rwidth, int rheight)
		: nanogui::Screen(pwindow, Eigen::Vector2i(rwidth, rheight), "SDL_gui Test")
	{
		mProgress = nullptr;
		using namespace nanogui;

		{
			auto& nwindow = add<Window>("Button demo");
			nwindow.setPosition(Vector2i(15, 15));
			nwindow.setLayout(new GroupLayout());

			nwindow.add<Label>("Push buttons", "sans-bold");
			nwindow.add<Button>("Plain button").withCallback([] { cout << "pushed!" << endl; });

			nwindow.add<Button>("Styled", ENTYPO_ICON_ROCKET).withCallback([] { cout << "pushed!" << endl; })
				.withBackgroundColor(Color(0, 0, 255, 25));

			nwindow.add<Label>("Toggle buttons", "sans-bold");
			nwindow.add<Button>("Toggle me").withFlags(Button::ToggleButton)
				.withChangeCallback([](bool state) { cout << "Toggle button state: " << state << endl; });

			nwindow.add<Label>("Radio buttons", "sans-bold");
			nwindow.add<Button>("Radio button 1").withFlags(Button::RadioButton);

			nwindow.add<Button>("Radio button 2").withFlags(Button::RadioButton);
			nwindow.add<Label>("A tool palette", "sans-bold");

			auto& tools = nwindow.add<Widget>().withLayout<BoxLayout>(Orientation::Horizontal,
				Alignment::Middle, 0, 6);

			tools.add<ToolButton>(ENTYPO_ICON_CLOUD);
			tools.add<ToolButton>(ENTYPO_ICON_FF);
			tools.add<ToolButton>(ENTYPO_ICON_COMPASS);
			tools.add<ToolButton>(ENTYPO_ICON_INSTALL);

			nwindow.add<Label>("Popup buttons", "sans-bold");
			PopupButton& pButton = nwindow.add<PopupButton>("Popup", ENTYPO_ICON_EXPORT);
			pButton.popup()->withLayout<GroupLayout>();
			pButton.popup()->add<Label>("Arbitrary widgets can be placed here");
			pButton.popup()->add<CheckBox>("A check box");

			PopupButton& rPopupBtn = pButton.popup()->add<PopupButton>("Recursive popup", ENTYPO_ICON_FLASH);
			rPopupBtn.popup()->withLayout<GroupLayout>();
			rPopupBtn.popup()->add<CheckBox>("Another check box");
		}
		char cCurrentPath[FILENAME_MAX];

		_getcwd(cCurrentPath, sizeof(cCurrentPath));

		cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */

		std::cout << "The current working directory is " << cCurrentPath << std::endl;

		std::vector<std::pair<int, std::string>>
			icons = loadImageDirectory(mNVGContext, "resources/icons");

		{
			auto& window = add<Window>("Basic widgets")
				.withPosition(Vector2i(200, 15))
				.withLayout<GroupLayout>();

			window.add<Label>("Message dialog", "sans-bold");
			auto& tools = window.add<Widget>()
				.withLayout<BoxLayout>(Orientation::Horizontal,
					Alignment::Middle, 0, 6);
			tools.add<Button>("Info")
				.withCallback([&] {
				add<MessageDialog>(MessageDialog::Type::Information, "Title", "This is an information message")
					.withCallback([](int result) { cout << "Dialog result: " << result << endl; });
			});
			tools.add<Button>("Warn")
				.withCallback([&]
			{
				add<MessageDialog>(MessageDialog::Type::Warning, "Title", "This is a warning message")
					.withCallback([](int result) { cout << "Dialog result: " << result << endl; });
			}
			);
			tools.add<Button>("Ask")
				.withCallback([&] {
				add<MessageDialog>(MessageDialog::Type::Warning, "Title", "This is a question message", "Yes", "No", true)
					.withCallback([](int result) { cout << "Dialog result: " << result << endl; });
			});

			window.add<Label>("Image panel & scroll panel", "sans-bold");
			PopupButton& imagePanelBtn = window.add<PopupButton>("Image Panel");

			imagePanelBtn.setIcon(ENTYPO_ICON_FOLDER);
			auto* popup = imagePanelBtn.popup();
			VScrollPanel& vscroll = popup->add<VScrollPanel>();
			ImagePanel& imgPanel = vscroll.add<ImagePanel>()
				.withImages(icons);

			popup->setFixedSize(Vector2i(245, 150));

			auto& img_window = add<Window>("Selected image");
			img_window.withPosition(Vector2i(675, 15))
				.withLayout<GroupLayout>();

			auto& img = img_window.add<ImageView>()
				.withPolicy(ImageView::SizePolicy::Expand)
				.withImage(icons[0].first);
			img.setFixedSize(Vector2i(300, 300));
			imgPanel.setCallback([&img, &imgPanel, &imagePanelBtn](int i) {
				img.setImage(imgPanel.images()[i].first); cout << "Selected item " << i << endl;
			});

			auto& img_cb = img_window.add<CheckBox>("Expand",
				[&img](bool state) { if (state) img.setPolicy(ImageView::SizePolicy::Expand);
				else       img.setPolicy(ImageView::SizePolicy::Fixed); });
			img_cb.setChecked(true);
			window.add<Label>("File dialog", "sans-bold");
			auto& tools2 = window.add<Widget>().withLayout<BoxLayout>(Orientation::Horizontal,
				Alignment::Middle, 0, 6);
			tools2.add<Button>("Open")
				.withCallback([&] {
				cout << "File dialog result: " << file_dialog(
				{ { "png", "Portable Network Graphics" },{ "txt", "Text file" } }, false) << endl;
			});

			tools2.add<Button>("Save").withCallback([&] {
				cout << "File dialog result: " << file_dialog(
				{ { "png", "Portable Network Graphics" },{ "txt", "Text file" } }, true) << endl;
			});

			window.add<Label>("Combo box", "sans-bold");
			window.add<ComboBox>(std::vector<std::string>{ "Combo box item 1", "Combo box item 2", "Combo box item 3"});
			window.add<Label>("Check box", "sans-bold");

			auto& cb = window.add<CheckBox>("Flag 1",
				[](bool state) { cout << "Check box 1 state: " << state << endl; }
			);
			cb.setChecked(true);
			window.add<CheckBox>("Flag 2",
				[](bool state) { cout << "Check box 2 state: " << state << endl; }
			);
			window.add<Label>("Progress bar", "sans-bold");
			mProgress = &window.add<ProgressBar>();

			window.add<Label>("Slider and text box", "sans-bold");

			auto& panel = window.add<Widget>()
				.withLayout<BoxLayout>(Orientation::Horizontal,
					Alignment::Middle, 0, 20);

			auto& slider = panel.add<Slider>();
			slider.setValue(0.5f);
			slider.setFixedWidth(80);

			auto& textBox = panel.add<TextBox>();
			textBox.setFixedSize(Vector2i(60, 25));
			textBox.setValue("50");
			textBox.setUnits("%");
			slider.setCallback([&textBox](float value) {
				textBox.setValue(std::to_string((int)(value * 100)));
			});
			slider.setFinalCallback([](float value) {
				cout << "Final slider value: " << (int)(value * 100) << endl;
			});
			textBox.setFixedSize(Vector2i(60, 25));
			textBox.setFontSize(20);
			textBox.setAlignment(TextBox::Alignment::Right);
		}

		{
			auto& window = add<Window>("Misc. widgets");
			window.setPosition(Vector2i(425, 15));
			window.setLayout(new GroupLayout());
			window.add<Label>("Color wheel", "sans-bold");
			window.add<ColorWheel>();
			window.add<Label>("Function graph", "sans-bold");
			Graph& graph = window.add<Graph>("Some function");
			graph.setHeader("E = 2.35e-3");
			graph.setFooter("Iteration 89");
			VectorXf &func = graph.values();
			func.resize(100);

			for (int i = 0; i < 100; ++i)
				func[i] = 0.5f * (0.5f * std::sin(i / 10.f) +
					0.5f * std::cos(i / 23.f) + 1);


		}

		{
			auto& window = add<Window>("Grid of small widgets");
			window.withPosition({ 425, 288 });
			auto* layout = new GridLayout(Orientation::Horizontal, 2,
				Alignment::Middle, 15, 5);
			layout->setColAlignment({ Alignment::Maximum, Alignment::Fill });
			layout->setSpacing(0, 10);
			window.setLayout(layout);

			window.add<Label>("Floating point :", "sans-bold");
			auto& textBox = window.add<TextBox>();
			textBox.setEditable(true);
			textBox.setFixedSize(Vector2i(100, 20));
			textBox.setValue("50");
			textBox.setUnits("GiB");
			textBox.setDefaultValue("0.0");
			textBox.setFontSize(16);
			textBox.setFormat("[-]?[0-9]*\\.?[0-9]+");

			window.add<Label>("Positive integer :", "sans-bold");
			auto& textBox2 = window.add<TextBox>();
			textBox2.setEditable(true);
			textBox2.setFixedSize(Vector2i(100, 20));
			textBox2.setValue("50");
			textBox2.setUnits("Mhz");
			textBox2.setDefaultValue("0.0");
			textBox2.setFontSize(16);
			textBox2.setFormat("[1-9][0-9]*");

			window.add<Label>("Checkbox :", "sans-bold");

			window.add<CheckBox>("Check me")
				.withChecked(true)
				.withFontSize(16);

			window.add<Label>("Combo box :", "sans-bold");
			window.add<ComboBox>()
				.withItems(std::vector<std::string>{ "Item 1", "Item 2", "Item 3" })
				.withFontSize(16)
				.withFixedSize(Vector2i(100, 20));

			window.add<Label>("Color button :", "sans-bold");
			auto& popupBtn = window.add<PopupButton>("", 0);
			popupBtn.setBackgroundColor(Color(255, 120, 0, 255));
			popupBtn.setFontSize(16);
			popupBtn.setFixedSize(Vector2i(100, 20));
			auto& popup = popupBtn.popup()->withLayout<GroupLayout>();

			ColorWheel& colorwheel = popup.add<ColorWheel>();
			colorwheel.setColor(popupBtn.backgroundColor());

			Button& colorBtn = popup.add<Button>("Pick");
			colorBtn.setFixedSize(Vector2i(100, 25));
			Color c = colorwheel.color();
			colorBtn.setBackgroundColor(c);

			colorwheel.setCallback([&colorBtn](const Color &value) {
				colorBtn.setBackgroundColor(value);
			});

			colorBtn.setChangeCallback([&colorBtn, &popupBtn](bool pushed) {
				if (pushed) {
					popupBtn.setBackgroundColor(colorBtn.backgroundColor());
					popupBtn.setPushed(false);
				}
			});
		}
		performLayout(mNVGContext);
	}

	~GUI()
	{
	}

	virtual bool keyboardEvent(int key, int scancode, int action, int modifiers)
	{
		if (Screen::keyboardEvent(key, scancode, action, modifiers))
			return true;

		return false;
	}

	virtual void draw(NVGcontext *ctx) {
		// Animate the scrollbar
		if (mProgress)
		{
			mProgress->setValue(mProgress->value() + 0.001f);
			if (mProgress->value() >= 1.f)
				mProgress->setValue(0.f);
		}

		// Draw the user interface
		Screen::draw(ctx);
	}

	virtual void drawContents()
	{
	}

	void MessageBoxAsk(const std::string & title, const std::string & msg)
	{
		add<MessageDialog>(MessageDialog::Type::Warning, title, msg, "Yes", "No", true)
			.withCallback([](int result) { cout << "Dialog result: " << result << endl; });
	}

private:
	nanogui::ProgressBar *mProgress;
};
