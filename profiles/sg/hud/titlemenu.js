root.TitleMenu = {
    Type: TitleMenuType.SG,
    DrawType: DrawComponentType.SystemMenu,
    BackgroundSprite: "TitleMenuBackground",
    PressToStartX: 830,
    PressToStartY: 620,
    PressToStartAnimDurationIn: 0.7,
    PressToStartAnimDurationOut: 0.7,
    PressToStartAnimFastDurationIn: 0.1,
    PressToStartAnimFastDurationOut: 0.1,
    PressToStartSprite: "TitleMenuPressToStart",
    BackgroundX: 0.0,
    BackgroundY: 0.0,
    // TODO: Replace with real value
    MenuEntriesNum: 0
};

root.Sprites["TitleMenuBackground"] = {
    Sheet: "Title",
    Bounds: { X: 0, Y: 0, Width: 1920, Height: 1080 },
};

root.Sprites["TitleMenuPressToStart"] = {
    Sheet: "Title",
    Bounds: { X: 140, Y: 1080, Width: 360, Height: 50 },
};