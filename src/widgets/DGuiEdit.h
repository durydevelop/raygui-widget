#ifndef DGuiEdit_H
#define DGuiEdit_H

#include <DGuiWidget.h>

class DGuiEdit : public DGuiWidget
{
    public:
        DGuiEdit(int LeftPos, int TopPos, int ControlWidth, int ControlHeight, DGuiWidget *ParentWidget);
        DGuiEdit(Rectangle WidgetBounds, DGuiWidget *ParentWidget);

        void SetMaxTextLenght(size_t Lenght);
        size_t GetMaxTextLenght(void);
        void ClearText(void);

        void SetReadOnly(bool Enabled);
        bool GetReadOnly(void);
        void SetPasswordMode(bool Enabled);
        bool GetPasswordMode(void);

        void Draw() override;
        int DrawTextBox(Rectangle bounds, char *text, int bufferSize, bool editMode);
        const std::string& GetText(void) override;
        char* GetEditText(void);

    private:
        std::string Text; // overrided
        size_t MaxTextLenght;
        bool EditMode;
        bool ReadOnly;
        bool PasswordMode;
        char* t; // Need a dedicated text buffer because DrawTextBox cannot handle std::string.

        //int DrawTextBox(Rectangle bounds, int bufferSize, bool editMode);
};

#endif