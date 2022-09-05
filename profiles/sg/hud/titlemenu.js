root.TitleMenu = {
    Type: TitleMenuType.SG,
    DrawType: DrawComponentType.SystemMenu,
    BackgroundSprite: "TitleMenuBackground",
    PressToStartX: 744,
    PressToStartY: 586,
    PressToStartAnimDurationIn: 0.7,
    PressToStartAnimDurationOut: 0.7,
    PressToStartSprite: "TitleMenuPressToStart",
    BackgroundX: 0.0,
    BackgroundY: 0.0,
    // TODO: Replace with real value
    MenuEntriesNum: 0
};

// TODO: Replace with real values
root.Sprites["TitleMenuBackground"] = {
    Sheet: "Title",
    Bounds: { X: 0, Y: 0, Width: 1920, Height: 1080 },
};

// TODO: Replace with real values
root.Sprites["TitleMenuPressToStart"] = {
    Sheet: "Title",
    Bounds: { X: 0, Y: 308, Width: 430, Height: 80 },
};