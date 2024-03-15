#ifndef DGuiLabel_H
#define DGuiLabel_H

#include <raygui.h>
#include <DGuiWidget.h>

class DGuiLabel : public DGuiWidget
{
    public:
        DGuiLabel(int LeftPos, int TopPos, int ControlWidth, int ControlHeight, DGuiWidget *ParentWidget);
        DGuiLabel(Rectangle WidgetBounds, DGuiWidget *ParentWidget);

        void SetPrefix(std::string PrefixText);
        void SetSuffix(std::string SuffixText);

        void ClearText(void);

        void Draw() override;

    private:
        std::string TextPrefix;
        std::string TextSuffix;
};

#endif