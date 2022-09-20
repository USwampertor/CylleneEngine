/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
/**
 * @file cyOmniChannel.h
 * @author Marco "Swampy" Millan
 * @date 20/09/2022
 * @brief 
 * 
 */
/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
#pragma once

#include "cyOmniversePrerequisites.h"

namespace CYLLENE_SDK
{

// Forward declaration
class OmniChannelMessageImpl;

/**
 * OMNIMESSAGETYPE
 * Description: A simple enum for messages being sent and received
 * 
 * Sample usage: 
 * 	
 */
namespace OMNIMESSAGETYPE
{
  BETTER_ENUM(E, uint32, 
              eJOIN, 
              eHELLO, 
              eGETUSER, 
              eLEFT, 
              eMERGESTARTED, 
              eMERGEFINISHED,
              eNONE);
}

/*
 * Omniverse Channel Message class
 *
 * This class describes a JSON-formatted message that is sent and received through
 * the Omniverse channel file contained in the live session folder.
*/
class OmniChannelMessage
{
public:

  /**
   * Default constructor
   */
  OmniChannelMessage();
  
  /**
   * Constructor setting the information of the message
   */
  OmniChannelMessage(OmniClientChannelEvent eventType, 
                     const char* fromUserId, 
                     struct OmniClientContent* content);
  
  /**
   * Copy constructor
   */
  OmniChannelMessage(const OmniChannelMessage& src);

  /**
   * Default destructor
   */
  ~OmniChannelMessage();


  // Setup a message that's been received
  OmniClientContent 
  GetOmniClientContent();
  
  void 
  BuildMessage(OMNIMESSAGETYPE::E messageType, 
               const char* fromUserName, 
               const char* app);
  
  String 
  GetVersion() const;
  
  String 
  GetFromUserId() const;
  
  String 
  GetFromUser() const;
  
  String 
  GetApp() const;
  
  OMNIMESSAGETYPE::E 
  GetType() const;
  
  void 
  SetOmniChannelEventType(OmniClientChannelEvent event);
  
  OmniClientChannelEvent 
  GetOmniChannelEventType() const;

  static OMNIMESSAGETYPE::E 
  StringTypeToMessageType(const String& msgStr) {
    return OMNIMESSAGETYPE::E::_from_string_nothrow(msgStr.c_str()) ? 
            OMNIMESSAGETYPE::E::_from_string(msgStr.c_str()) : OMNIMESSAGETYPE::E::eNONE;
  }

  static const char* 
  MessageTypeToStringType(OMNIMESSAGETYPE::E msgType) {
    return msgType._to_string();
  }


  static constexpr const char* MSG_HEADER = "__OVUM__";
  static constexpr const size_t MSG_HEADER_LEN = 8;
  static constexpr const char* MSG_VERSION = "3.0";
  static constexpr const char* CUSTOM_MSG_TYPE = "message";
  static constexpr const char* CUSTOM_MSG_GROUP_KEY = "__SESSION_MANAGEMENT__";
  static constexpr const char* CUSTOM_MSG_VERSION = "1.0";

  static constexpr const char* MSG_VERSION_KEY = "version";
  static constexpr const char* MSG_USERNAME_KEY = "from_user_name";
  static constexpr const char* MSG_APP_KEY = "app";
  static constexpr const char* MSG_TYPE_KEY = "message_type";
  static constexpr const char* MSG_CONTENT_KEY = "content";

private:
  const OmniChannelMessageImpl* 
  Pimpl() const { return m_pImpl.get(); }
  
  OmniChannelMessageImpl* 
  Pimpl() { return m_pImpl.get(); }

  UniquePointer<OmniChannelMessageImpl> m_pImpl;
};



/*
 * Omniverse Peer User struct
 *
 * This struct wraps the items that denote a peer user in the live session.
 *
 *  userName: the login name for the connected user
 *  app:      the application used to connect to the live session
 *  userId:   the Nucleus user id, unique to every connection (regardless of the user or application)
*/
struct OmniPeerUser
{
  OmniPeerUser(const String& id, const String& name, const String& a)
    : userId(id)
    , userName(name)
    , app(a)
  {}

  struct Compare {
    bool operator()(const OmniPeerUser& lhs, const OmniPeerUser& rhs) const {
      return (lhs.userName + lhs.app + lhs.userId) < (rhs.userName + rhs.app + rhs.userId);
    }
  };

  String userId;
  String userName;
  String app;
};


/*
 * Omniverse Channel class
 *
 * This class contains methods to access the Omniverse live session channel for communicating
 * user presence, application name, and merge status.
 */
class OmniChannelImpl;
class OmniChannel
{
public:
  typedef void (NotifyCallback)(OMNIMESSAGETYPE::E messageType, 
                                void* userData, 
                                const char* userName, 
                                const char* appName);
  
  using MessageVector = Vector<OmniChannelMessage>;

  /**
   * Default constructor
   */
  OmniChannel();

  /**
   * Default destructor
   */
  ~OmniChannel();

  void 
  SetChannelUrl(const char* joinChannelUrl);

  void 
  SetAppName(const char* appName);
  
  void 
  RegisterNotifyCallback(NotifyCallback& cb, void* userData = nullptr);
  
  void 
  ClearNotifyCallback();
  
  bool 
  JoinChannel();
  
  void 
  LeaveChannel();
  
  bool 
  IsConnected();

  // This method needs to be periodically called by the application to flush messages
  // from the queue and call notify callbacks
  void 
  Update();

  // If an application wants to poll the channel to process and fetch messages
  // (rather than have Update issue callbacks)
  MessageVector 
  GetAllMessages();

  void 
  SendChannelMessage(OMNIMESSAGETYPE::E messageType);
  
  size_t 
  GetUsersCount() const;
  
  const OmniPeerUser& 
  GetUserAtIndex(size_t index) const;

private:
  const OmniChannelImpl* 
  Pimpl() const { return m_pImpl.get(); }
  
  OmniChannelImpl* 
  Pimpl() { return m_pImpl.get(); }
  
  UniquePointer<OmniChannelImpl> m_pImpl;
};


}

