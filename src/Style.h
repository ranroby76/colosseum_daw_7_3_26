#pragma once

#include <JuceHeader.h>

namespace Style {
    const auto colBackground = juce::Colour(0xff1e1e1e);
    const auto colNodeBg = juce::Colour(0xff2d2d2d);
    const auto colNodeBypassed = juce::Colour(0xff3a3a3a);
    const auto colNodeBodyBypassed = juce::Colour(0xff3a3a3a);  // Body color when bypassed
    const auto colIONode = juce::Colour(0xff404040);
    const auto colNodeBody   = juce::Colour(0xff2d2d2d);
    const auto colNodeHeader = juce::Colour(0xff3a3a3a);
    const auto colNodeBorder = juce::Colour(0xff555555);
    const auto colNodeBorderActive = juce::Colour(0xff00aaff);
    const auto colNodeTitle = juce::Colour(0xff505050);
    const auto colNodeTitleBypassed = juce::Colour(0xff404040);  // Title color when bypassed
    const auto colText       = juce::Colours::grey;
    const auto colPinAudio     = juce::Colour(0xff00aaff);  // Blue
    const auto colPinSidechain = juce::Colour(0xff00ff00);  // Pure Green
    const auto colPinMidi      = juce::Colour(0xffff0000);  // Pure Red
    const auto colBypass       = juce::Colour(0xffd63031);
    
    const float nodeHeight = 60.0f;
    const float nodeWidth = 152.0f;     // FIX: wider to fit up to 6 buttons (VST2: E+M+P+T+L+X)
    const float nodeCornerSize = 5.0f;
    const float nodeRounding = 5.0f;  // Rounding radius for node corners
    const float nodeTitleHeight = 24.0f;
    const float pinSize = 8.0f;
    const float hookLength = 10.0f; 
    const float btnSize = 14.0f;
    const float pinSpacing = 14.0f;
    const float minNodeWidth = 156.0f;  // FIX: minimum with margin for 6 buttons
    const float minPinSpacing = 18.0f;
    
    const float bottomBtnWidth = 20.0f;
    const float bottomBtnHeight = 20.0f;
    const float bottomBtnSpacing = 4.0f;
    const float bottomBtnMargin = 4.0f;
    
    const int mainHeaderHeight = 60;  // Doubled from 30 for logo display
    const int instrHeaderHeight = 140;  // FIX: Doubled from 70 for taller instrument buttons
    const int rightMenuWidth = 96;
    const int leftMenuWidth = 96;  // FIX: Left green tab menu
    const auto colLeftMenu = juce::Colour(0xff2E7D32);  // FIX: Dark green (darker than before)
}

// =============================================================================
// ColosseumLookAndFeel — Black-Green themed AlertWindows with logo
// Applied globally to replace default blue-cyan JUCE dialogs
// =============================================================================
class ColosseumLookAndFeel : public juce::LookAndFeel_V4
{
public:
    ColosseumLookAndFeel()
    {
        // =====================================================================
        // ONLY AlertWindow-specific color IDs — these do NOT bleed into other UI
        // Everything else is handled in draw overrides with parent-type checks
        // =====================================================================
        setColour(juce::AlertWindow::backgroundColourId, juce::Colour(0xff141414));
        setColour(juce::AlertWindow::textColourId, juce::Colour(0xff4CAF50));
        setColour(juce::AlertWindow::outlineColourId, juce::Colour(0xff2E7D32));

        // Load logo from known paths
        loadLogo();
    }
    
    void drawAlertBox(juce::Graphics& g, juce::AlertWindow& alert,
                      const juce::Rectangle<int>& textArea,
                      juce::TextLayout& textLayout) override
    {
        auto bounds = alert.getLocalBounds().toFloat();
        
        // =====================================================================
        // Background: dark fill with green border — sharp corners
        // =====================================================================
        g.setColour(juce::Colour(0xff141414));
        g.fillRect(bounds);
        
        // Outer border — green
        g.setColour(juce::Colour(0xff2E7D32));
        g.drawRect(bounds.reduced(0.5f), 1.5f);
        
        // Inner subtle border
        g.setColour(juce::Colour(0xff1a3a1a));
        g.drawRect(bounds.reduced(2.0f), 0.5f);
        
        // =====================================================================
        // Header bar — dark green gradient across the top, sharp corners
        // =====================================================================
        auto headerRect = bounds.removeFromTop(48.0f);
        juce::ColourGradient headerGrad(juce::Colour(0xff1a2e1a), headerRect.getX(), headerRect.getY(),
                                         juce::Colour(0xff0f1a0f), headerRect.getX(), headerRect.getBottom(), false);
        g.setGradientFill(headerGrad);
        g.fillRect(headerRect);
        
        // Separator line under header
        g.setColour(juce::Colour(0xff2E7D32).withAlpha(0.6f));
        g.drawHorizontalLine((int)headerRect.getBottom(), headerRect.getX() + 8.0f, headerRect.getRight() - 8.0f);
        
        // =====================================================================
        // Logo — upper right corner of header
        // =====================================================================
        if (logo.isValid())
        {
            float logoH = 36.0f;
            float logoW = logoH * ((float)logo.getWidth() / (float)logo.getHeight());
            float logoX = headerRect.getRight() - logoW - 10.0f;
            float logoY = headerRect.getY() + (48.0f - logoH) * 0.5f;
            
            g.setOpacity(0.95f);
            g.drawImage(logo, logoX, logoY, logoW, logoH,
                        0, 0, logo.getWidth(), logo.getHeight());
            g.setOpacity(1.0f);
        }
        
        // =====================================================================
        // Message body text — green
        // =====================================================================
        auto bodyArea = textArea.toFloat();
        bodyArea.setY(bodyArea.getY() + 12.0f);
        
        textLayout.draw(g, bodyArea);
    }
    
    // =========================================================================
    // AlertWindow button styling — sharp green rectangles
    // =========================================================================
    void drawButtonBackground(juce::Graphics& g, juce::Button& button,
                              const juce::Colour& backgroundColour,
                              bool shouldDrawButtonAsHighlighted,
                              bool shouldDrawButtonAsDown) override
    {
        auto bounds = button.getLocalBounds().toFloat().reduced(0.5f);
        
        // Check if this button is inside an AlertWindow
        bool inAlertWindow = (dynamic_cast<juce::AlertWindow*>(button.getParentComponent()) != nullptr);
        
        if (inAlertWindow)
        {
            juce::Colour base = juce::Colour(0xff2E7D32);
            
            if (shouldDrawButtonAsDown)
                base = juce::Colour(0xff1B5E20);
            else if (shouldDrawButtonAsHighlighted)
                base = juce::Colour(0xff388E3C);
            
            g.setColour(base);
            g.fillRect(bounds);
            
            g.setColour(juce::Colour(0xff4CAF50).withAlpha(0.4f));
            g.drawRect(bounds, 1.0f);
        }
        else
        {
            // Default V4 behavior for non-AlertWindow buttons
            juce::LookAndFeel_V4::drawButtonBackground(g, button, backgroundColour,
                                                        shouldDrawButtonAsHighlighted,
                                                        shouldDrawButtonAsDown);
        }
    }
    
    // =========================================================================
    // Button text — max contrast rule:
    //   AlertWindow buttons: light green on dark green
    //   All others: white text on dark backgrounds, black text on bright backgrounds
    // =========================================================================
    void drawButtonText(juce::Graphics& g, juce::TextButton& button,
                        bool shouldDrawButtonAsHighlighted,
                        bool shouldDrawButtonAsDown) override
    {
        bool inAlertWindow = (dynamic_cast<juce::AlertWindow*>(button.getParentComponent()) != nullptr);
        
        if (inAlertWindow)
        {
            auto bounds = button.getLocalBounds();
            g.setFont(juce::Font(juce::FontOptions(14.0f, juce::Font::bold)));
            g.setColour(juce::Colour(0xffb9f6ca));  // Light green text on dark green button
            g.drawText(button.getButtonText(), bounds, juce::Justification::centred, true);
        }
        else
        {
            // Determine the button's actual background color
            auto bgColour = button.findColour(button.getToggleState()
                                              ? juce::TextButton::buttonOnColourId
                                              : juce::TextButton::buttonColourId);
            
            // Max contrast: dark bg → white text, bright bg → black text
            // Threshold at 0.55 — covers dark reds, dark greens, dark blues
            bool isDark = bgColour.getBrightness() < 0.55f;
            juce::Colour textColour = isDark ? juce::Colours::white : juce::Colours::black;
            
            // Scale font to fit button width — important for narrow buttons
            auto bounds = button.getLocalBounds().reduced(2);
            float maxFontSize = juce::jmin(15.0f, (float)bounds.getHeight() * 0.6f);
            auto font = juce::Font(juce::FontOptions(maxFontSize, juce::Font::bold));
            
            // Shrink font if text doesn't fit width
            float textWidth = font.getStringWidthFloat(button.getButtonText());
            if (textWidth > (float)bounds.getWidth() && bounds.getWidth() > 0)
            {
                float scale = (float)bounds.getWidth() / textWidth;
                float newSize = juce::jmax(8.0f, maxFontSize * scale);
                font = juce::Font(juce::FontOptions(newSize, juce::Font::bold));
            }
            
            g.setFont(font);
            g.setColour(textColour);
            g.drawFittedText(button.getButtonText(), bounds, juce::Justification::centred,
                             juce::jmax(1, bounds.getHeight() / (int)font.getHeight()));
        }
    }
    
    // =========================================================================
    // Reload logo (call if assets path changes)
    // =========================================================================
    void loadLogo()
    {
        // Try multiple known paths for the Colosseum logo
        juce::StringArray paths = {
            juce::File::getSpecialLocation(juce::File::currentApplicationFile)
                .getParentDirectory().getChildFile("assets").getChildFile("colosseum_logo.png").getFullPathName(),
            juce::File::getSpecialLocation(juce::File::currentApplicationFile)
                .getParentDirectory().getChildFile("colosseum_logo.png").getFullPathName(),
            "D:/Workspace/Subterraneum_plugins_daw/assets/colosseum_logo.png"
        };
        
        #if JUCE_MAC
        // macOS: also check inside bundle Resources
        paths.add(juce::File::getSpecialLocation(juce::File::currentApplicationFile)
                      .getChildFile("Contents/Resources/colosseum_logo.png").getFullPathName());
        #endif
        
        for (const auto& path : paths)
        {
            juce::File f(path);
            if (f.existsAsFile())
            {
                logo = juce::ImageFileFormat::loadFrom(f);
                if (logo.isValid())
                    return;
            }
        }
    }
    
    juce::Image logo;
};

