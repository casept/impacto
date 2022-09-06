root.TitleMenu = {
    Type: TitleMenuType.SG,
    DrawType: DrawComponentType.SystemMenu,
    BackgroundSprite: "TitleMenuBackground",
    BackgroundX: 0.0,
    BackgroundY: 0.0,

    PressToStartX: 830,
    PressToStartY: 620,
    PressToStartAnimDurationIn: 0.7,
    PressToStartAnimDurationOut: 0.7,
    PressToStartAnimFastDurationIn: 0.1,
    PressToStartAnimFastDurationOut: 0.1,
    PressToStartSprite: "TitleMenuPressToStart",

    MenuX: 1555,
    MenuY: 110,
    MenuEntriesNum: 5,
    MenuEntriesSprites: ["TitleMenuEntryStart", "TitleMenuEntryLoad", "TitleMenuEntryExtra", "TitleMenuEntryConfig", "TitleMenuEntryHelp"],
    MenuEntriesHighlightedSprites: ["TitleMenuEntryStartHighlighted", "TitleMenuEntryLoadHighlighted", "TitleMenuEntryExtraHighlighted", "TitleMenuEntryConfigHighlighted", "TitleMenuEntryHelpHighlighted"],
    MenuEntriesSpacingY: 75,
    MenuStartId: 0,
    MenuLoadId: 1,
    MenuExtraId: 2,
    MenuConfigId: 3,
    MenuHelpId: 4,
};

root.Sprites["TitleMenuBackground"] = {
    Sheet: "Title",
    Bounds: { X: 0, Y: 0, Width: 1920, Height: 1080 },
};

root.Sprites["TitleMenuPressToStart"] = {
    Sheet: "Title",
    Bounds: { X: 140, Y: 1080, Width: 360, Height: 50 },
};

const MenuEntryWidth = 200;
const MenuEntryHeight = 50;
const MenuEntrySheet = "Title";
const MenuEntryFirstRowX = 1920;
const MenuEntrySecondRowX = 1920 + MenuEntryWidth;
const MenuEntryThirdRowX = 1920 + MenuEntryWidth * 2;
const MenuEntryFourthRowX = 1920 + MenuEntryWidth * 3;

root.Sprites["TitleMenuEntryStartHighlighted"] = {
    Sheet: MenuEntrySheet,
    Bounds: { X: MenuEntryFirstRowX, Y: 0, Width: MenuEntryWidth, Height: MenuEntryHeight },
};

root.Sprites["TitleMenuEntryLoadHighlighted"] = {
    Sheet: MenuEntrySheet,
    Bounds: { X: MenuEntryFirstRowX, Y: MenuEntryHeight * 1, Width: MenuEntryWidth, Height: MenuEntryHeight },
};

root.Sprites["TitleMenuEntryExtraHighlighted"] = {
    Sheet: MenuEntrySheet,
    Bounds: { X: MenuEntryFirstRowX, Y: MenuEntryHeight * 2, Width: MenuEntryWidth, Height: MenuEntryHeight },
};

root.Sprites["TitleMenuEntryConfigHighlighted"] = {
    Sheet: MenuEntrySheet,
    Bounds: { X: MenuEntryFirstRowX, Y: MenuEntryHeight * 3, Width: MenuEntryWidth, Height: MenuEntryHeight },
};

root.Sprites["TitleMenuEntryHelpHighlighted"] = {
    Sheet: MenuEntrySheet,
    Bounds: { X: MenuEntryThirdRowX, Y: 0, Width: MenuEntryWidth, Height: MenuEntryHeight },
};

root.Sprites["TitleMenuEntryStart"] = {
    Sheet: MenuEntrySheet,
    Bounds: { X: MenuEntrySecondRowX, Y: 0, Width: MenuEntryWidth, Height: MenuEntryHeight },
};

root.Sprites["TitleMenuEntryLoad"] = {
    Sheet: MenuEntrySheet,
    Bounds: { X: MenuEntrySecondRowX, Y: MenuEntryHeight * 1, Width: MenuEntryWidth, Height: MenuEntryHeight },
};

root.Sprites["TitleMenuEntryExtra"] = {
    Sheet: MenuEntrySheet,
    Bounds: { X: MenuEntrySecondRowX, Y: MenuEntryHeight * 2, Width: MenuEntryWidth, Height: MenuEntryHeight },
};

root.Sprites["TitleMenuEntryConfig"] = {
    Sheet: MenuEntrySheet,
    Bounds: { X: MenuEntrySecondRowX, Y: MenuEntryHeight * 3, Width: MenuEntryWidth, Height: MenuEntryHeight },
};

root.Sprites["TitleMenuEntryHelp"] = {
    Sheet: MenuEntrySheet,
    Bounds: { X: MenuEntryFourthRowX, Y: 0, Width: MenuEntryWidth, Height: MenuEntryHeight },
};