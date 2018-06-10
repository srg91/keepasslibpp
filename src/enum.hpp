#pragma once

#include <bitset>

namespace keepasslibpp {

// TODO: look up enum class name conventions
/*
 * Compression algorithm specifiers.
 */
enum class PwCompressionAlgorithm {
    // No compression.
        None,

    // GZip compression.
        GZip,

    // TODO: Is we need this type?
    // Virtual field: currently known number of algorithms. Should not be used
    // by plugins or libraries -- it's used internally only.
        Count
};

/*
 * Tree traversal methods.
 */
enum class TraversalMethod {
    // Don't traverse the tree.
        None,

    // Traverse thre tree in pre-order mode, i.e. first visit all items
    // in the current node, then visit all sub-nodes.
        PreOrder
};

/*
 * Methods for merging password databases/entries.
 */
enum class PwMergeMethod {
    None,
    OverwriteExisting,
    KeepExisting,
    OverwriteIfNewer,
    CreateNewUuids,
    Synchronize
};

/*
 *  Icon identifiers for groups and password entries.
 */
enum class PwIcon {
    Key,
    World,
    Warning,
    NetworkServer,
    MarkedDirectory,
    UserCommunication,
    Parts,
    Notepad,
    WorldSocket,
    Identity,
    PaperReady,
    Digicam,
    IRCommunication,
    MultiKeys,
    Energy,
    Scanner,
    WorldStar,
    CDRom,
    Monitor,
    EMail,
    Configuration,
    ClipboardReady,
    PaperNew,
    Screen,
    EnergyCareful,
    EMailBox,
    Disk,
    Drive,
    PaperQ,
    TerminalEncrypted,
    Console,
    Printer,
    ProgramIcons,
    Run,
    Settings,
    WorldComputer,
    Archive,
    Homebanking,
    DriveWindows,
    Clock,
    EMailSearch,
    PaperFlag,
    Memory,
    TrashBin,
    Note,
    Expired,
    Info,
    Package,
    Folder,
    FolderOpen,
    FolderPackage,
    LockOpen,
    PaperLocked,
    Checked,
    Pen,
    Thumbnail,
    Book,
    List,
    UserKey,
    Tool,
    Home,
    Star,
    Tux,
    Feather,
    Apple,
    Wiki,
    Money,
    Certificate,
    BlackBerry,

    // Virtual identifier -- represents the number of icons.
        Count
};

// TODO: Get rid of this
// ProxyServerType
// ProxyAuthType

/*
 * Comparison modes for in-memory protected objects.
 */
enum class MemProtCmpMode {
    // Ignore the in-memory protection states.
        None,

    // Ignore the in-memory protection states of standard
    // objects; do compare in-memory protection states of
    // custom objects.
        CustomOnly,

    // Compare in-memory protection states.
        Full
};

// TODO: Enum class with | or |= operators?
//namespace PwCompareOptions {
//    enum PwCompareOptions {
//        None = 0x0,
//        NullEmptyEquivStd = 0x1,
//        IgnoreParentGroup = 0x2,
//        IgnoreLastAccess = 0x4,
//        IgnoreLastMod = 0x8,
//        IgnoreHistory = 0x10,
//        IgnoreLastBackup = 0x20,
//        // For groups:
//        PropertiesOnly = 0x40,
//
//        IgnoreTimes = (IgnoreLastAccess | IgnoreLastMod)
//    };
//}

// TODO: Use bitset like in this article:
// TODO: Static class with method
// https://habr.com/post/334988/
enum class PwCompareOptions {
    None,

    // Empty standard string fields are considered to be the
    // same as non-existing standard string fields.
    // This doesn't affect custom string comparisons.
        NullEmptyEquivStd,
    IgnoreParentGroup,
    IgnoreLastAccess,
    IgnoreLastMod,
    IgnoreHistory,
    IgnoreLastBackup,
    ProPropertiesOnly,
    // virtual
        Count,
    // TODO: IgnoreTimes = (IgnoreLastAccess | IgnoreLastMod)
};

// TODO: Get rid of it
// IOAccessType

// TODO: Get rid of it
namespace AppRunFlags {
    enum AppRunFlags {
        None = 0,
        GetStdOutput = 1,
        WaitForExit = 2,
        GCKeepAlive = 4,
        DoEvents = 8,
        DisableForms = 16
    };
}

enum class ScaleTransformFlags {
    None,
    // UIIcon indicates that the returned image is going
    // to be displayed as icon in the UI and that it is not
    // subject to future changes in size.
        UIIcon
};

// TODO: Get rid of it
enum class DesktopType {
    None,
    Windows,
    Gnome,
    Kde,
    Unity,
    Lxde,
    Xfce,
    Mate,
    Cinnamon,
    Pantheon
};

}