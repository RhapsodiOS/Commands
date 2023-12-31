# Generated from 'Movies.h'

def FOUR_CHAR_CODE(x): return x
MovieFileType = FOUR_CHAR_CODE('MooV')
MovieScrapType = FOUR_CHAR_CODE('moov')
MovieResourceType = FOUR_CHAR_CODE('moov')
MovieForwardPointerResourceType = FOUR_CHAR_CODE('fore')
MovieBackwardPointerResourceType = FOUR_CHAR_CODE('back')
MovieResourceAtomType = FOUR_CHAR_CODE('moov')
MovieDataAtomType = FOUR_CHAR_CODE('mdat')
FreeAtomType = FOUR_CHAR_CODE('free')
SkipAtomType = FOUR_CHAR_CODE('skip')
WideAtomPlaceholderType = FOUR_CHAR_CODE('wide')
MediaHandlerType = FOUR_CHAR_CODE('mhlr')
DataHandlerType = FOUR_CHAR_CODE('dhlr')
VideoMediaType = FOUR_CHAR_CODE('vide')
SoundMediaType = FOUR_CHAR_CODE('soun')
TextMediaType = FOUR_CHAR_CODE('text')
BaseMediaType = FOUR_CHAR_CODE('gnrc')
MPEGMediaType = FOUR_CHAR_CODE('MPEG')
MusicMediaType = FOUR_CHAR_CODE('musi')
TimeCodeMediaType = FOUR_CHAR_CODE('tmcd')
SpriteMediaType = FOUR_CHAR_CODE('sprt')
FlashMediaType = FOUR_CHAR_CODE('flsh')
MovieMediaType = FOUR_CHAR_CODE('moov')
TweenMediaType = FOUR_CHAR_CODE('twen')
ThreeDeeMediaType = FOUR_CHAR_CODE('qd3d')
HandleDataHandlerSubType = FOUR_CHAR_CODE('hndl')
ResourceDataHandlerSubType = FOUR_CHAR_CODE('rsrc')
URLDataHandlerSubType = FOUR_CHAR_CODE('url ')
WiredActionHandlerType = FOUR_CHAR_CODE('wire')
VisualMediaCharacteristic = FOUR_CHAR_CODE('eyes')
AudioMediaCharacteristic = FOUR_CHAR_CODE('ears')
kCharacteristicCanSendVideo = FOUR_CHAR_CODE('vsnd')
kCharacteristicProvidesActions = FOUR_CHAR_CODE('actn')
kCharacteristicNonLinear = FOUR_CHAR_CODE('nonl')
kCharacteristicCanStep = FOUR_CHAR_CODE('step')
kCharacteristicHasNoDuration = FOUR_CHAR_CODE('noti')
kUserDataMovieControllerType = FOUR_CHAR_CODE('ctyp')
kUserDataName = FOUR_CHAR_CODE('name')
kUserDataTextAuthor = FOUR_CHAR_CODE('�aut')
kUserDataTextComment = FOUR_CHAR_CODE('�cmt')
kUserDataTextCopyright = FOUR_CHAR_CODE('�cpy')
kUserDataTextCreationDate = FOUR_CHAR_CODE('�day')
kUserDataTextDescription = FOUR_CHAR_CODE('�des')
kUserDataTextDirector = FOUR_CHAR_CODE('�dir')
kUserDataTextDisclaimer = FOUR_CHAR_CODE('�dis')
kUserDataTextFullName = FOUR_CHAR_CODE('�nam')
kUserDataTextHostComputer = FOUR_CHAR_CODE('�hst')
kUserDataTextInformation = FOUR_CHAR_CODE('�inf')
kUserDataTextKeywords = FOUR_CHAR_CODE('�key')
kUserDataTextMake = FOUR_CHAR_CODE('�mak')
kUserDataTextModel = FOUR_CHAR_CODE('�mod')
kUserDataTextOriginalFormat = FOUR_CHAR_CODE('�fmt')
kUserDataTextOriginalSource = FOUR_CHAR_CODE('�src')
kUserDataTextPerformers = FOUR_CHAR_CODE('�prf')
kUserDataTextProducer = FOUR_CHAR_CODE('�prd')
kUserDataTextProduct = FOUR_CHAR_CODE('�PRD')
kUserDataTextSoftware = FOUR_CHAR_CODE('�swr')
kUserDataTextSpecialPlaybackRequirements = FOUR_CHAR_CODE('�req')
kUserDataTextWarning = FOUR_CHAR_CODE('�wrn')
kUserDataTextWriter = FOUR_CHAR_CODE('�wrt')
kUserDataTextEditDate1 = FOUR_CHAR_CODE('�ed1')
kUserDataTextChapter = FOUR_CHAR_CODE('�chp')
kUserDataUnicodeBit = 1L << 7
DoTheRightThing = 0
kMusicFlagDontPlay2Soft = 1L << 0
kMusicFlagDontSlaveToMovie = 1L << 1
dfDontDisplay = 1 << 0
dfDontAutoScale = 1 << 1
dfClipToTextBox = 1 << 2
dfUseMovieBGColor = 1 << 3
dfShrinkTextBoxToFit = 1 << 4
dfScrollIn = 1 << 5
dfScrollOut = 1 << 6
dfHorizScroll = 1 << 7
dfReverseScroll = 1 << 8
dfContinuousScroll = 1 << 9
dfFlowHoriz = 1 << 10
dfContinuousKaraoke = 1 << 11
dfDropShadow = 1 << 12
dfAntiAlias = 1 << 13
dfKeyedText = 1 << 14
dfInverseHilite = 1 << 15
dfTextColorHilite = 1 << 16                       
searchTextDontGoToFoundTime = 1L << 16
searchTextDontHiliteFoundText = 1L << 17
searchTextOneTrackOnly = 1L << 18
searchTextEnabledTracksOnly = 1L << 19
kTextTextHandle = 1
kTextTextPtr = 2
kTextTEStyle = 3
kTextSetSelection = 4
kTextBackColor = 5
kTextForeColor = 6
kTextFace = 7
kTextFont = 8
kTextSize = 9
kTextAlignment = 10
kTextHilite = 11
kTextDropShadow = 12
kTextDisplayFlags = 13
kTextScroll = 14
k3DMediaRendererEntry = FOUR_CHAR_CODE('rend')
k3DMediaRendererName = FOUR_CHAR_CODE('name')
k3DMediaRendererCode = FOUR_CHAR_CODE('rcod')
movieProgressOpen = 0
movieProgressUpdatePercent = 1
movieProgressClose = 2
progressOpFlatten = 1
progressOpInsertTrackSegment = 2
progressOpInsertMovieSegment = 3
progressOpPaste = 4
progressOpAddMovieSelection = 5
progressOpCopy = 6
progressOpCut = 7
progressOpLoadMovieIntoRam = 8
progressOpLoadTrackIntoRam = 9
progressOpLoadMediaIntoRam = 10
progressOpImportMovie = 11
progressOpExportMovie = 12
mediaQualityDraft = 0x0000
mediaQualityNormal = 0x0040
mediaQualityBetter = 0x0080
mediaQualityBest = 0x00C0
kActionMovieSetVolume = 1024
kActionMovieSetRate = 1025
kActionMovieSetLoopingFlags = 1026
kActionMovieGoToTime = 1027
kActionMovieGoToTimeByName = 1028
kActionMovieGoToBeginning = 1029
kActionMovieGoToEnd = 1030
kActionMovieStepForward = 1031
kActionMovieStepBackward = 1032
kActionMovieSetSelection = 1033
kActionMovieSetSelectionByName = 1034
kActionMoviePlaySelection = 1035
kActionMovieSetLanguage = 1036
kActionMovieChanged = 1037
kActionMovieRestartAtTime = 1038
kActionTrackSetVolume = 2048
kActionTrackSetBalance = 2049
kActionTrackSetEnabled = 2050
kActionTrackSetMatrix = 2051
kActionTrackSetLayer = 2052
kActionTrackSetClip = 2053
kActionTrackSetCursor = 2054
kActionTrackSetGraphicsMode = 2055
kActionSpriteSetMatrix = 3072
kActionSpriteSetImageIndex = 3073
kActionSpriteSetVisible = 3074
kActionSpriteSetLayer = 3075
kActionSpriteSetGraphicsMode = 3076
kActionSpritePassMouseToCodec = 3078
kActionSpriteClickOnCodec = 3079
kActionSpriteTranslate = 3080
kActionSpriteScale = 3081
kActionSpriteRotate = 3082
kActionSpriteStretch = 3083
kActionQTVRSetPanAngle = 4096
kActionQTVRSetTiltAngle = 4097
kActionQTVRSetFieldOfView = 4098
kActionQTVRShowDefaultView = 4099
kActionQTVRGoToNodeID = 4100
kActionMusicPlayNote = 5120
kActionMusicSetController = 5121
kActionCase = 6144
kActionWhile = 6145
kActionGoToURL = 6146
kActionSendQTEventToSprite = 6147
kActionDebugStr = 6148
kActionPushCurrentTime = 6149
kActionPushCurrentTimeWithLabel = 6150
kActionPopAndGotoTopTime = 6151
kActionPopAndGotoLabeledTime = 6152
kActionStatusString = 6153
kActionSendQTEventToTrackObject = 6154
kActionAddChannelSubscription = 6155
kActionRemoveChannelSubscription = 6156
kActionOpenCustomActionHandler = 6157
kActionDoScript = 6158
kActionSpriteTrackSetVariable = 7168
kActionSpriteTrackNewSprite = 7169
kActionSpriteTrackDisposeSprite = 7170
kActionSpriteTrackSetVariableToString = 7171
kActionSpriteTrackConcatVariables = 7172
kActionSpriteTrackSetVariableToMovieURL = 7173
kActionSpriteTrackSetVariableToMovieBaseURL = 7174
kActionApplicationNumberAndString = 8192
kActionQD3DNamedObjectTranslateTo = 9216
kActionQD3DNamedObjectScaleTo = 9217
kActionQD3DNamedObjectRotateTo = 9218
kActionFlashTrackSetPan = 10240
kActionFlashTrackSetZoom = 10241
kActionFlashTrackSetZoomRect = 10242
kActionFlashTrackGotoFrameNumber = 10243
kActionFlashTrackGotoFrameLabel = 10244
kActionMovieTrackAddChildMovie = 11264
kActionMovieTrackLoadChildMovie = 11265                     
kOperandExpression = 1
kOperandConstant = 2
kOperandSubscribedToChannel = 3
kOperandUniqueCustomActionHandlerID = 4
kOperandCustomActionHandlerIDIsOpen = 5
kOperandConnectionSpeed = 6
kOperandGMTDay = 7
kOperandGMTMonth = 8
kOperandGMTYear = 9
kOperandGMTHours = 10
kOperandGMTMinutes = 11
kOperandGMTSeconds = 12
kOperandLocalDay = 13
kOperandLocalMonth = 14
kOperandLocalYear = 15
kOperandLocalHours = 16
kOperandLocalMinutes = 17
kOperandLocalSeconds = 18
kOperandRegisteredForQuickTimePro = 19
kOperandPlatformRunningOn = 20
kOperandQuickTimeVersion = 21
kOperandComponentVersion = 22
kOperandOriginalHandlerRefcon = 23
kOperandTicks = 24
kOperandMaxLoadedTimeInMovie = 25
kOperandMovieVolume = 1024
kOperandMovieRate = 1025
kOperandMovieIsLooping = 1026
kOperandMovieLoopIsPalindrome = 1027
kOperandMovieTime = 1028
kOperandTrackVolume = 2048
kOperandTrackBalance = 2049
kOperandTrackEnabled = 2050
kOperandTrackLayer = 2051
kOperandTrackWidth = 2052
kOperandTrackHeight = 2053
kOperandSpriteBoundsLeft = 3072
kOperandSpriteBoundsTop = 3073
kOperandSpriteBoundsRight = 3074
kOperandSpriteBoundsBottom = 3075
kOperandSpriteImageIndex = 3076
kOperandSpriteVisible = 3077
kOperandSpriteLayer = 3078
kOperandSpriteTrackVariable = 3079
kOperandSpriteTrackNumSprites = 3080
kOperandSpriteTrackNumImages = 3081
kOperandSpriteID = 3082
kOperandSpriteIndex = 3083
kOperandSpriteFirstCornerX = 3084
kOperandSpriteFirstCornerY = 3085
kOperandSpriteSecondCornerX = 3086
kOperandSpriteSecondCornerY = 3087
kOperandSpriteThirdCornerX = 3088
kOperandSpriteThirdCornerY = 3089
kOperandSpriteFourthCornerX = 3090
kOperandSpriteFourthCornerY = 3091
kOperandSpriteImageRegistrationPointX = 3092
kOperandSpriteImageRegistrationPointY = 3093
kOperandQTVRPanAngle = 4096
kOperandQTVRTiltAngle = 4097
kOperandQTVRFieldOfView = 4098
kOperandQTVRNodeID = 4099
kOperandMouseLocalHLoc = 5120
kOperandMouseLocalVLoc = 5121
kOperandKeyIsDown = 5122
kOperandRandom = 5123                          
kFirstMovieAction = kActionMovieSetVolume
kLastMovieAction = kActionMovieRestartAtTime
kFirstTrackAction = kActionTrackSetVolume
kLastTrackAction = kActionTrackSetGraphicsMode
kFirstSpriteAction = kActionSpriteSetMatrix
kLastSpriteAction = kActionSpriteStretch
kFirstQTVRAction = kActionQTVRSetPanAngle
kLastQTVRAction = kActionQTVRGoToNodeID
kFirstMusicAction = kActionMusicPlayNote
kLastMusicAction = kActionMusicSetController
kFirstSystemAction = kActionCase
kLastSystemAction = kActionDoScript
kFirstSpriteTrackAction = kActionSpriteTrackSetVariable
kLastSpriteTrackAction = kActionSpriteTrackSetVariableToMovieBaseURL
kFirstApplicationAction = kActionApplicationNumberAndString
kLastApplicationAction = kActionApplicationNumberAndString
kFirstQD3DNamedObjectAction = kActionQD3DNamedObjectTranslateTo
kLastQD3DNamedObjectAction = kActionQD3DNamedObjectRotateTo
kFirstFlashTrackAction = kActionFlashTrackSetPan
kLastFlashTrackAction = kActionFlashTrackGotoFrameLabel
kFirstMovieTrackAction = kActionMovieTrackAddChildMovie
kLastMovieTrackAction = kActionMovieTrackLoadChildMovie
kFirstAction = kFirstMovieAction
kLastAction = kActionMovieTrackLoadChildMovie
kTargetMovie = FOUR_CHAR_CODE('moov')
kTargetMovieName = FOUR_CHAR_CODE('mona')
kTargetMovieID = FOUR_CHAR_CODE('moid')
kTargetRootMovie = FOUR_CHAR_CODE('moro')
kTargetParentMovie = FOUR_CHAR_CODE('mopa')
kTargetChildMovieTrackName = FOUR_CHAR_CODE('motn')
kTargetChildMovieTrackID = FOUR_CHAR_CODE('moti')
kTargetChildMovieTrackIndex = FOUR_CHAR_CODE('motx')
kTargetChildMovieMovieName = FOUR_CHAR_CODE('momn')
kTargetChildMovieMovieID = FOUR_CHAR_CODE('momi')
kTargetTrackName = FOUR_CHAR_CODE('trna')
kTargetTrackID = FOUR_CHAR_CODE('trid')
kTargetTrackType = FOUR_CHAR_CODE('trty')
kTargetTrackIndex = FOUR_CHAR_CODE('trin')
kTargetSpriteName = FOUR_CHAR_CODE('spna')
kTargetSpriteID = FOUR_CHAR_CODE('spid')
kTargetSpriteIndex = FOUR_CHAR_CODE('spin')
kTargetQD3DNamedObjectName = FOUR_CHAR_CODE('nana')        
kQTEventType = FOUR_CHAR_CODE('evnt')
kAction = FOUR_CHAR_CODE('actn')
kWhichAction = FOUR_CHAR_CODE('whic')
kActionParameter = FOUR_CHAR_CODE('parm')
kActionTarget = FOUR_CHAR_CODE('targ')
kActionFlags = FOUR_CHAR_CODE('flag')
kActionParameterMinValue = FOUR_CHAR_CODE('minv')
kActionParameterMaxValue = FOUR_CHAR_CODE('maxv')
kActionListAtomType = FOUR_CHAR_CODE('list')
kExpressionContainerAtomType = FOUR_CHAR_CODE('expr')
kConditionalAtomType = FOUR_CHAR_CODE('test')
kOperatorAtomType = FOUR_CHAR_CODE('oper')
kOperandAtomType = FOUR_CHAR_CODE('oprn')
kCommentAtomType = FOUR_CHAR_CODE('why ')
kCustomActionHandler = FOUR_CHAR_CODE('cust')
kCustomHandlerID = FOUR_CHAR_CODE('id  ')
kCustomHandlerDesc = FOUR_CHAR_CODE('desc')
kQTEventRecordAtomType = FOUR_CHAR_CODE('erec')
kQTEventMouseClick = FOUR_CHAR_CODE('clik')
kQTEventMouseClickEnd = FOUR_CHAR_CODE('cend')
kQTEventMouseClickEndTriggerButton = FOUR_CHAR_CODE('trig')
kQTEventMouseEnter = FOUR_CHAR_CODE('entr')
kQTEventMouseExit = FOUR_CHAR_CODE('exit')
kQTEventMouseMoved = FOUR_CHAR_CODE('move')
kQTEventFrameLoaded = FOUR_CHAR_CODE('fram')
kQTEventIdle = FOUR_CHAR_CODE('idle')
kQTEventKey = FOUR_CHAR_CODE('key ')
kQTEventMovieLoaded = FOUR_CHAR_CODE('load')
kQTEventRequestToModifyMovie = FOUR_CHAR_CODE('reqm')
kActionFlagActionIsDelta = 1L << 1
kActionFlagParameterWrapsAround = 1L << 2
kActionFlagActionIsToggle = 1L << 3
kStatusStringIsURLLink = 1L << 1
kStatusStringIsStreamingStatus = 1L << 2
kStatusHasCodeNumber = 1L << 3
kStatusIsError = 1L << 4
kScriptIsUnknownType = 1L << 0
kScriptIsJavaScript = 1L << 1
kScriptIsLingoEvent = 1L << 2
kScriptIsVBEvent = 1L << 3
kScriptIsProjectorCommand = 1L << 4
kQTRegistrationDialogTimeOutFlag = 1 << 0
kQTRegistrationDialogShowDialog = 1 << 1
kQTRegistrationDialogForceDialog = 1 << 2
kOperatorAdd = FOUR_CHAR_CODE('add ')
kOperatorSubtract = FOUR_CHAR_CODE('sub ')
kOperatorMultiply = FOUR_CHAR_CODE('mult')
kOperatorDivide = FOUR_CHAR_CODE('div ')
kOperatorOr = FOUR_CHAR_CODE('or  ')
kOperatorAnd = FOUR_CHAR_CODE('and ')
kOperatorNot = FOUR_CHAR_CODE('not ')
kOperatorLessThan = FOUR_CHAR_CODE('<   ')
kOperatorLessThanEqualTo = FOUR_CHAR_CODE('<=  ')
kOperatorEqualTo = FOUR_CHAR_CODE('=   ')
kOperatorNotEqualTo = FOUR_CHAR_CODE('!=  ')
kOperatorGreaterThan = FOUR_CHAR_CODE('>   ')
kOperatorGreaterThanEqualTo = FOUR_CHAR_CODE('>=  ')
kOperatorModulo = FOUR_CHAR_CODE('mod ')
kOperatorIntegerDivide = FOUR_CHAR_CODE('idiv')
kOperatorAbsoluteValue = FOUR_CHAR_CODE('abs ')
kOperatorNegate = FOUR_CHAR_CODE('neg ')
kPlatformMacintosh = 1
kPlatformWindows = 2
kMediaPropertyNonLinearAtomType = FOUR_CHAR_CODE('nonl')
kMediaPropertyHasActions = 105
loopTimeBase = 1
palindromeLoopTimeBase = 2
maintainTimeBaseZero = 4
triggerTimeFwd = 0x0001
triggerTimeBwd = 0x0002
triggerTimeEither = 0x0003
triggerRateLT = 0x0004
triggerRateGT = 0x0008
triggerRateEqual = 0x0010
triggerRateLTE = triggerRateLT | triggerRateEqual
triggerRateGTE = triggerRateGT | triggerRateEqual
triggerRateNotEqual = triggerRateGT | triggerRateEqual | triggerRateLT
triggerRateChange = 0
triggerAtStart = 0x0001
triggerAtStop = 0x0002
timeBaseBeforeStartTime = 1
timeBaseAfterStopTime = 2
callBackAtTime = 1
callBackAtRate = 2
callBackAtTimeJump = 3
callBackAtExtremes = 4
callBackAtTimeBaseDisposed = 5
callBackAtInterrupt = 0x8000
callBackAtDeferredTask = 0x4000
qtcbNeedsRateChanges = 1
qtcbNeedsTimeChanges = 2
qtcbNeedsStartStopChanges = 4                             
keepInRam = 1 << 0
unkeepInRam = 1 << 1
flushFromRam = 1 << 2
loadForwardTrackEdits = 1 << 3
loadBackwardTrackEdits = 1 << 4                        
newMovieActive = 1 << 0
newMovieDontResolveDataRefs = 1 << 1
newMovieDontAskUnresolvedDataRefs = 1 << 2
newMovieDontAutoAlternates = 1 << 3
newMovieDontUpdateForeBackPointers = 1 << 4
newMovieDontAutoUpdateClock = 1 << 5
newMovieAsyncOK = 1 << 8
newMovieIdleImportOK = 1 << 10
trackUsageInMovie = 1 << 1
trackUsageInPreview = 1 << 2
trackUsageInPoster = 1 << 3
mediaSampleNotSync = 1 << 0
mediaSampleShadowSync = 1 << 1                        
pasteInParallel = 1 << 0
showUserSettingsDialog = 1 << 1
movieToFileOnlyExport = 1 << 2
movieFileSpecValid = 1 << 3
nextTimeMediaSample = 1 << 0
nextTimeMediaEdit = 1 << 1
nextTimeTrackEdit = 1 << 2
nextTimeSyncSample = 1 << 3
nextTimeStep = 1 << 4
nextTimeEdgeOK = 1 << 14
nextTimeIgnoreActiveSegment = 1 << 15
createMovieFileDeleteCurFile = 1L << 31
createMovieFileDontCreateMovie = 1L << 30
createMovieFileDontOpenFile = 1L << 29
createMovieFileDontCreateResFile = 1L << 28
flattenAddMovieToDataFork = 1L << 0
flattenActiveTracksOnly = 1L << 2
flattenDontInterleaveFlatten = 1L << 3
flattenFSSpecPtrIsDataRefRecordPtr = 1L << 4
flattenCompressMovieResource = 1L << 5
flattenForceMovieResourceBeforeMovieData = 1L << 6
movieInDataForkResID = -1                            
mcTopLeftMovie = 1 << 0
mcScaleMovieToFit = 1 << 1
mcWithBadge = 1 << 2
mcNotVisible = 1 << 3
mcWithFrame = 1 << 4                        
movieScrapDontZeroScrap = 1 << 0
movieScrapOnlyPutMovie = 1 << 1
dataRefSelfReference = 1 << 0
dataRefWasNotResolved = 1 << 1
kMovieAnchorDataRefIsDefault = 1 << 0                       
hintsScrubMode = 1 << 0
hintsLoop = 1 << 1
hintsDontPurge = 1 << 2
hintsUseScreenBuffer = 1 << 5
hintsAllowInterlace = 1 << 6
hintsUseSoundInterp = 1 << 7
hintsHighQuality = 1 << 8
hintsPalindrome = 1 << 9
hintsInactive = 1 << 11
hintsOffscreen = 1 << 12
hintsDontDraw = 1 << 13
hintsAllowBlacklining = 1 << 14
hintsDontUseVideoOverlaySurface = 1 << 16
hintsIgnoreBandwidthRestrictions = 1 << 17
hintsPlayingEveryFrame = 1 << 18
hintsAllowDynamicResize = 1 << 19
hintsSingleField = 1 << 20
mediaHandlerFlagBaseClient = 1
movieTrackMediaType = 1 << 0
movieTrackCharacteristic = 1 << 1
movieTrackEnabledOnly = 1 << 2
movieDrawingCallWhenChanged = 0
movieDrawingCallAlways = 1
kGetMovieImporterValidateToFind = 1L << 0
kGetMovieImporterAllowNewFile = 1L << 1
kGetMovieImporterDontConsiderGraphicsImporters = 1L << 2
kGetMovieImporterDontConsiderFileOnlyImporters = 1L << 6
kMovieLoadStateError = -1L
kMovieLoadStateLoading = 1000
kMovieLoadStatePlayable = 10000
kMovieLoadStateComplete = 100000L
preloadAlways = 1L << 0
preloadOnlyIfEnabled = 1L << 1
fullScreenHideCursor = 1L << 0
fullScreenAllowEvents = 1L << 1
fullScreenDontChangeMenuBar = 1L << 2
fullScreenPreflightSize = 1L << 3
movieExecuteWiredActionDontExecute = 1L << 0
kBackgroundSpriteLayerNum = 32767
kSpritePropertyMatrix = 1
kSpritePropertyImageDescription = 2
kSpritePropertyImageDataPtr = 3
kSpritePropertyVisible = 4
kSpritePropertyLayer = 5
kSpritePropertyGraphicsMode = 6
kSpritePropertyImageDataSize = 7
kSpritePropertyActionHandlingSpriteID = 8
kSpritePropertyImageIndex = 100
kSpriteTrackPropertyBackgroundColor = 101
kSpriteTrackPropertyOffscreenBitDepth = 102
kSpriteTrackPropertySampleFormat = 103
kSpriteTrackPropertyScaleSpritesToScaleWorld = 104
kSpriteTrackPropertyHasActions = 105
kSpriteTrackPropertyVisible = 106
kSpriteTrackPropertyQTIdleEventsFrequency = 107
kSpriteImagePropertyRegistrationPoint = 1000
kSpriteImagePropertyGroupID = 1001
kNoQTIdleEvents = -1
kOnlyDrawToSpriteWorld = 1L << 0
kSpriteWorldPreflight = 1L << 1
kSpriteWorldDidDraw = 1L << 0
kSpriteWorldNeedsToDraw = 1L << 1
kKeyFrameAndSingleOverride = 1L << 1
kKeyFrameAndAllOverrides = 1L << 2
kScaleSpritesToScaleWorld = 1L << 1
kSpriteWorldHighQuality = 1L << 2
kSpriteWorldDontAutoInvalidate = 1L << 3
kSpriteWorldInvisible = 1L << 4
kParentAtomIsContainer = 0
kEffectNameAtom = FOUR_CHAR_CODE('name')
kEffectTypeAtom = FOUR_CHAR_CODE('type')
kEffectManufacturerAtom = FOUR_CHAR_CODE('manu')        
pdActionConfirmDialog = 1
pdActionSetAppleMenu = 2
pdActionSetEditMenu = 3
pdActionGetDialogValues = 4
pdActionSetPreviewUserItem = 5
pdActionSetPreviewPicture = 6
pdActionSetColorPickerEventProc = 7
pdActionSetDialogTitle = 8
pdActionGetSubPanelMenu = 9
pdActionActivateSubPanel = 10
pdActionConductStopAlert = 11
pdActionModelessCallback = 12
pdActionFetchPreview = 13                            
elOptionsIncludeNoneInList = 0x00000001                    
pdOptionsCollectOneValue = 0x00000001
pdOptionsAllowOptionalInterpolations = 0x00000002
pdOptionsModalDialogBox = 0x00000004                    
effectIsRealtime = 0                             
kAccessKeyAtomType = FOUR_CHAR_CODE('acky')
kAccessKeySystemFlag = 1L << 0
ConnectionSpeedPrefsType = FOUR_CHAR_CODE('cspd')
BandwidthManagementPrefsType = FOUR_CHAR_CODE('bwmg')
kQTIdlePriority = 10
kQTNonRealTimePriority = 20
kQTRealTimeSharedPriority = 25
kQTRealTimePriority = 30
kQTBandwidthNotifyNeedToStop = 1L << 0
kQTBandwidthNotifyGoodToGo = 1L << 1
kQTBandwidthChangeRequest = 1L << 2
kQTBandwidthQueueRequest = 1L << 3
kQTBandwidthScheduledRequest = 1L << 4
kQTBandwidthVoluntaryRelease = 1L << 5
kITextRemoveEverythingBut = 0 << 1
kITextRemoveLeaveSuggestedAlternate = 1 << 1
kITextAtomType = FOUR_CHAR_CODE('itxt')
kITextStringAtomType = FOUR_CHAR_CODE('text')
kQTParseTextHREFText = FOUR_CHAR_CODE('text')
kQTParseTextHREFBaseURL = FOUR_CHAR_CODE('burl')
kQTParseTextHREFClickPoint = FOUR_CHAR_CODE('clik')
kQTParseTextHREFUseAltDelim = FOUR_CHAR_CODE('altd')
kQTParseTextHREFDelimiter = FOUR_CHAR_CODE('delm')
kQTParseTextHREFRecomposeHREF = FOUR_CHAR_CODE('rhrf')      
kQTParseTextHREFURL = FOUR_CHAR_CODE('url ')
kQTParseTextHREFTarget = FOUR_CHAR_CODE('targ')
kQTParseTextHREFIsAutoHREF = FOUR_CHAR_CODE('auto')
kQTParseTextHREFIsServerMap = FOUR_CHAR_CODE('smap')
kQTParseTextHREFHREF = FOUR_CHAR_CODE('href')        
kTrackReferenceChapterList = FOUR_CHAR_CODE('chap')
kTrackReferenceTimeCode = FOUR_CHAR_CODE('tmcd')
kTrackReferenceModifier = FOUR_CHAR_CODE('ssrc')
kTrackModifierInput = 0x696E
kTrackModifierType = 0x7479
kTrackModifierReference = FOUR_CHAR_CODE('ssrc')
kTrackModifierObjectID = FOUR_CHAR_CODE('obid')
kTrackModifierInputName = FOUR_CHAR_CODE('name')
kInputMapSubInputID = FOUR_CHAR_CODE('subi')
kTrackModifierTypeMatrix = 1
kTrackModifierTypeClip = 2
kTrackModifierTypeGraphicsMode = 5
kTrackModifierTypeVolume = 3
kTrackModifierTypeBalance = 4
kTrackModifierTypeImage = FOUR_CHAR_CODE('vide')
kTrackModifierObjectMatrix = 6
kTrackModifierObjectGraphicsMode = 7
kTrackModifierType3d4x4Matrix = 8
kTrackModifierCameraData = 9
kTrackModifierSoundLocalizationData = 10
kTrackModifierObjectImageIndex = 11
kTrackModifierObjectLayer = 12
kTrackModifierObjectVisible = 13
kTrackModifierAngleAspectCamera = 14
kTrackModifierPanAngle = FOUR_CHAR_CODE('pan ')
kTrackModifierTiltAngle = FOUR_CHAR_CODE('tilt')
kTrackModifierVerticalFieldOfViewAngle = FOUR_CHAR_CODE('fov ')
kTrackModifierObjectQTEventSend = FOUR_CHAR_CODE('evnt')
kTweenTypeShort = 1
kTweenTypeLong = 2
kTweenTypeFixed = 3
kTweenTypePoint = 4
kTweenTypeQDRect = 5
kTweenTypeQDRegion = 6
kTweenTypeMatrix = 7
kTweenTypeRGBColor = 8
kTweenTypeGraphicsModeWithRGBColor = 9
kTweenTypeQTFloatSingle = 10
kTweenTypeQTFloatDouble = 11
kTweenTypeFixedPoint = 12
kTweenType3dScale = FOUR_CHAR_CODE('3sca')
kTweenType3dTranslate = FOUR_CHAR_CODE('3tra')
kTweenType3dRotate = FOUR_CHAR_CODE('3rot')
kTweenType3dRotateAboutPoint = FOUR_CHAR_CODE('3rap')
kTweenType3dRotateAboutAxis = FOUR_CHAR_CODE('3rax')
kTweenType3dRotateAboutVector = FOUR_CHAR_CODE('3rvc')
kTweenType3dQuaternion = FOUR_CHAR_CODE('3qua')
kTweenType3dMatrix = FOUR_CHAR_CODE('3mat')
kTweenType3dCameraData = FOUR_CHAR_CODE('3cam')
kTweenType3dAngleAspectCameraData = FOUR_CHAR_CODE('3caa')
kTweenType3dSoundLocalizationData = FOUR_CHAR_CODE('3slc')
kTweenTypePathToMatrixTranslation = FOUR_CHAR_CODE('gxmt')
kTweenTypePathToMatrixRotation = FOUR_CHAR_CODE('gxpr')
kTweenTypePathToMatrixTranslationAndRotation = FOUR_CHAR_CODE('gxmr')
kTweenTypePathToFixedPoint = FOUR_CHAR_CODE('gxfp')
kTweenTypePathXtoY = FOUR_CHAR_CODE('gxxy')
kTweenTypePathYtoX = FOUR_CHAR_CODE('gxyx')
kTweenTypeAtomList = FOUR_CHAR_CODE('atom')
kTweenTypePolygon = FOUR_CHAR_CODE('poly')
kTweenTypeMultiMatrix = FOUR_CHAR_CODE('mulm')
kTweenTypeSpin = FOUR_CHAR_CODE('spin')
kTweenType3dMatrixNonLinear = FOUR_CHAR_CODE('3nlr')
kTweenType3dVRObject = FOUR_CHAR_CODE('3vro')
kTweenEntry = FOUR_CHAR_CODE('twen')
kTweenData = FOUR_CHAR_CODE('data')
kTweenType = FOUR_CHAR_CODE('twnt')
kTweenStartOffset = FOUR_CHAR_CODE('twst')
kTweenDuration = FOUR_CHAR_CODE('twdu')
kTweenFlags = FOUR_CHAR_CODE('flag')
kTweenOutputMin = FOUR_CHAR_CODE('omin')
kTweenOutputMax = FOUR_CHAR_CODE('omax')
kTweenSequenceElement = FOUR_CHAR_CODE('seqe')
kTween3dInitialCondition = FOUR_CHAR_CODE('icnd')
kTweenInterpolationID = FOUR_CHAR_CODE('intr')
kTweenRegionData = FOUR_CHAR_CODE('qdrg')
kTweenPictureData = FOUR_CHAR_CODE('PICT')
kListElementType = FOUR_CHAR_CODE('type')
kListElementDataType = FOUR_CHAR_CODE('daty')
kNameAtom = FOUR_CHAR_CODE('name')
kInitialRotationAtom = FOUR_CHAR_CODE('inro')
kNonLinearTweenHeader = FOUR_CHAR_CODE('nlth')
kTweenReturnDelta = 1L << 0
videoFlagDontLeanAhead = 1L << 0
txtProcDefaultDisplay = 0
txtProcDontDisplay = 1
txtProcDoDisplay = 2                             
findTextEdgeOK = 1 << 0
findTextCaseSensitive = 1 << 1
findTextReverseSearch = 1 << 2
findTextWrapAround = 1 << 3
findTextUseOffset = 1 << 4                        
dropShadowOffsetType = FOUR_CHAR_CODE('drpo')
dropShadowTranslucencyType = FOUR_CHAR_CODE('drpt')
spriteHitTestBounds = 1L << 0
spriteHitTestImage = 1L << 1
spriteHitTestInvisibleSprites = 1L << 2
spriteHitTestIsClick = 1L << 3
spriteHitTestLocInDisplayCoordinates = 1L << 4              
kSpriteAtomType = FOUR_CHAR_CODE('sprt')
kSpriteImagesContainerAtomType = FOUR_CHAR_CODE('imct')
kSpriteImageAtomType = FOUR_CHAR_CODE('imag')
kSpriteImageDataAtomType = FOUR_CHAR_CODE('imda')
kSpriteImageDataRefAtomType = FOUR_CHAR_CODE('imre')
kSpriteImageDataRefTypeAtomType = FOUR_CHAR_CODE('imrt')
kSpriteImageGroupIDAtomType = FOUR_CHAR_CODE('imgr')
kSpriteImageRegistrationAtomType = FOUR_CHAR_CODE('imrg')
kSpriteImageDefaultImageIndexAtomType = FOUR_CHAR_CODE('defi')
kSpriteSharedDataAtomType = FOUR_CHAR_CODE('dflt')
kSpriteNameAtomType = FOUR_CHAR_CODE('name')
kSpriteImageNameAtomType = FOUR_CHAR_CODE('name')
kSpriteUsesImageIDsAtomType = FOUR_CHAR_CODE('uses')
kSpriteBehaviorsAtomType = FOUR_CHAR_CODE('beha')
kSpriteImageBehaviorAtomType = FOUR_CHAR_CODE('imag')
kSpriteCursorBehaviorAtomType = FOUR_CHAR_CODE('crsr')
kSpriteStatusStringsBehaviorAtomType = FOUR_CHAR_CODE('sstr')
kSpriteVariablesContainerAtomType = FOUR_CHAR_CODE('vars')
kSpriteStringVariableAtomType = FOUR_CHAR_CODE('strv')
kSpriteFloatingPointVariableAtomType = FOUR_CHAR_CODE('flov')
kMovieMediaDataReference = FOUR_CHAR_CODE('mmdr')
kMovieMediaDefaultDataReferenceID = FOUR_CHAR_CODE('ddri')
kMovieMediaSlaveTime = FOUR_CHAR_CODE('slti')
kMovieMediaSlaveAudio = FOUR_CHAR_CODE('slau')
kMovieMediaSlaveGraphicsMode = FOUR_CHAR_CODE('slgr')
kMovieMediaAutoPlay = FOUR_CHAR_CODE('play')
kMovieMediaLoop = FOUR_CHAR_CODE('loop')
kMovieMediaUseMIMEType = FOUR_CHAR_CODE('mime')
kMovieMediaTitle = FOUR_CHAR_CODE('titl')
kMovieMediaAltText = FOUR_CHAR_CODE('altt')
kMovieMediaClipBegin = FOUR_CHAR_CODE('clpb')
kMovieMediaClipDuration = FOUR_CHAR_CODE('clpd')
kMovieMediaRegionAtom = FOUR_CHAR_CODE('regi')
kMovieMediaSlaveTrackDuration = FOUR_CHAR_CODE('sltr')
kMovieMediaEnableFrameStepping = FOUR_CHAR_CODE('enfs')
kMovieMediaBackgroundColor = FOUR_CHAR_CODE('bkcl')        
kMovieMediaFitNone = 0
kMovieMediaFitScroll = FOUR_CHAR_CODE('scro')
kMovieMediaFitClipIfNecessary = FOUR_CHAR_CODE('hidd')
kMovieMediaFitFill = FOUR_CHAR_CODE('fill')
kMovieMediaFitMeet = FOUR_CHAR_CODE('meet')
kMovieMediaFitSlice = FOUR_CHAR_CODE('slic')
kMovieMediaSpatialAdjustment = FOUR_CHAR_CODE('fit ')
kMovieMediaRectangleAtom = FOUR_CHAR_CODE('rect')
kMovieMediaTop = FOUR_CHAR_CODE('top ')
kMovieMediaLeft = FOUR_CHAR_CODE('left')
kMovieMediaWidth = FOUR_CHAR_CODE('wd  ')
kMovieMediaHeight = FOUR_CHAR_CODE('ht  ')
kMoviePropertyDuration = FOUR_CHAR_CODE('dura')
kMoviePropertyTimeScale = FOUR_CHAR_CODE('tims')
kMoviePropertyTime = FOUR_CHAR_CODE('timv')
kMoviePropertyNaturalBounds = FOUR_CHAR_CODE('natb')
kMoviePropertyMatrix = FOUR_CHAR_CODE('mtrx')
kMoviePropertyTrackList = FOUR_CHAR_CODE('tlst')        
kTrackPropertyMediaType = FOUR_CHAR_CODE('mtyp')
kTrackPropertyInstantiation = FOUR_CHAR_CODE('inst')        
MovieControllerComponentType = FOUR_CHAR_CODE('play')
kMovieControllerQTVRFlag = 1 << 0
kMovieControllerDontDisplayToUser = 1 << 1
mcActionIdle = 1
mcActionDraw = 2
mcActionActivate = 3
mcActionDeactivate = 4
mcActionMouseDown = 5
mcActionKey = 6
mcActionPlay = 8
mcActionGoToTime = 12
mcActionSetVolume = 14
mcActionGetVolume = 15
mcActionStep = 18
mcActionSetLooping = 21
mcActionGetLooping = 22
mcActionSetLoopIsPalindrome = 23
mcActionGetLoopIsPalindrome = 24
mcActionSetGrowBoxBounds = 25
mcActionControllerSizeChanged = 26
mcActionSetSelectionBegin = 29
mcActionSetSelectionDuration = 30
mcActionSetKeysEnabled = 32
mcActionGetKeysEnabled = 33
mcActionSetPlaySelection = 34
mcActionGetPlaySelection = 35
mcActionSetUseBadge = 36
mcActionGetUseBadge = 37
mcActionSetFlags = 38
mcActionGetFlags = 39
mcActionSetPlayEveryFrame = 40
mcActionGetPlayEveryFrame = 41
mcActionGetPlayRate = 42
mcActionShowBalloon = 43
mcActionBadgeClick = 44
mcActionMovieClick = 45
mcActionSuspend = 46
mcActionResume = 47
mcActionSetControllerKeysEnabled = 48
mcActionGetTimeSliderRect = 49
mcActionMovieEdited = 50
mcActionGetDragEnabled = 51
mcActionSetDragEnabled = 52
mcActionGetSelectionBegin = 53
mcActionGetSelectionDuration = 54
mcActionPrerollAndPlay = 55
mcActionGetCursorSettingEnabled = 56
mcActionSetCursorSettingEnabled = 57
mcActionSetColorTable = 58
mcActionLinkToURL = 59
mcActionCustomButtonClick = 60
mcActionForceTimeTableUpdate = 61
mcActionSetControllerTimeLimits = 62
mcActionExecuteAllActionsForQTEvent = 63
mcActionExecuteOneActionForQTEvent = 64
mcActionAdjustCursor = 65
mcActionUseTrackForTimeTable = 66
mcActionClickAndHoldPoint = 67
mcActionShowMessageString = 68
mcActionShowStatusString = 69
mcActionGetExternalMovie = 70
mcActionGetChapterTime = 71
mcActionPerformActionList = 72
mcActionEvaluateExpression = 73
mcActionFetchParameterAs = 74
mcActionGetCursorByID = 75
mcActionGetNextURL = 76
mcActionMovieChanged = 77
mcActionDoScript = 78
mcActionRestartAtTime = 79
mcActionGetIndChapter = 80
mcActionLinkToURLExtended = 81                            
mcFlagSuppressMovieFrame = 1 << 0
mcFlagSuppressStepButtons = 1 << 1
mcFlagSuppressSpeakerButton = 1 << 2
mcFlagsUseWindowPalette = 1 << 3
mcFlagsDontInvalidate = 1 << 4
mcFlagsUseCustomButton = 1 << 5
mcPositionDontInvalidate = 1 << 5
kMCIEEnabledButtonPicture = 1
kMCIEDisabledButtonPicture = 2
kMCIEDepressedButtonPicture = 3
kMCIEEnabledSizeBoxPicture = 4
kMCIEDisabledSizeBoxPicture = 5
kMCIEEnabledUnavailableButtonPicture = 6
kMCIEDisabledUnavailableButtonPicture = 7
kMCIESoundSlider = 128
kMCIESoundThumb = 129
kMCIEColorTable = 256
kMCIEIsFlatAppearance = 257
kMCIEDoButtonIconsDropOnDepress = 258
mcInfoUndoAvailable = 1 << 0
mcInfoCutAvailable = 1 << 1
mcInfoCopyAvailable = 1 << 2
mcInfoPasteAvailable = 1 << 3
mcInfoClearAvailable = 1 << 4
mcInfoHasSound = 1 << 5
mcInfoIsPlaying = 1 << 6
mcInfoIsLooping = 1 << 7
mcInfoIsInPalindrome = 1 << 8
mcInfoEditingEnabled = 1 << 9
mcInfoMovieIsInteractive = 1 << 10
mcMenuUndo = 1
mcMenuCut = 3
mcMenuCopy = 4
mcMenuPaste = 5
mcMenuClear = 6
kFetchAsBooleanPtr = 1
kFetchAsShortPtr = 2
kFetchAsLongPtr = 3
kFetchAsMatrixRecordPtr = 4
kFetchAsModifierTrackGraphicsModeRecord = 5
kFetchAsHandle = 6
kFetchAsStr255 = 7
kFetchAsFloatPtr = 8
kFetchAsPointPtr = 9
kFetchAsNewAtomContainer = 10
kFetchAsQTEventRecordPtr = 11
kFetchAsFixedPtr = 12
kFetchAsSetControllerValuePtr = 13
kFetchAsRgnHandle = 14
kFetchAsComponentDescriptionPtr = 15
kFetchAsCString = 16
kQTCursorOpenHand = -19183
kQTCursorClosedHand = -19182
kQTCursorPointingHand = -19181
kQTCursorRightArrow = -19180
kQTCursorLeftArrow = -19179
kQTCursorDownArrow = -19178
kQTCursorUpArrow = -19177
kQTCursorIBeam = -19176
kVideoMediaResetStatisticsSelect = 0x0105
kVideoMediaGetStatisticsSelect = 0x0106
kVideoMediaGetStallCountSelect = 0x010E
kVideoMediaSetCodecParameterSelect = 0x010F
kVideoMediaGetCodecParameterSelect = 0x0110
kTextMediaSetTextProcSelect = 0x0101
kTextMediaAddTextSampleSelect = 0x0102
kTextMediaAddTESampleSelect = 0x0103
kTextMediaAddHiliteSampleSelect = 0x0104
kTextMediaDrawRawSelect = 0x0109
kTextMediaSetTextPropertySelect = 0x010A
kTextMediaRawSetupSelect = 0x010B
kTextMediaRawIdleSelect = 0x010C
kTextMediaFindNextTextSelect = 0x0105
kTextMediaHiliteTextSampleSelect = 0x0106
kTextMediaSetTextSampleDataSelect = 0x0107
kSpriteMediaSetPropertySelect = 0x0101
kSpriteMediaGetPropertySelect = 0x0102
kSpriteMediaHitTestSpritesSelect = 0x0103
kSpriteMediaCountSpritesSelect = 0x0104
kSpriteMediaCountImagesSelect = 0x0105
kSpriteMediaGetIndImageDescriptionSelect = 0x0106
kSpriteMediaGetDisplayedSampleNumberSelect = 0x0107
kSpriteMediaGetSpriteNameSelect = 0x0108
kSpriteMediaGetImageNameSelect = 0x0109
kSpriteMediaSetSpritePropertySelect = 0x010A
kSpriteMediaGetSpritePropertySelect = 0x010B
kSpriteMediaHitTestAllSpritesSelect = 0x010C
kSpriteMediaHitTestOneSpriteSelect = 0x010D
kSpriteMediaSpriteIndexToIDSelect = 0x010E
kSpriteMediaSpriteIDToIndexSelect = 0x010F
kSpriteMediaGetSpriteActionsForQTEventSelect = 0x0110
kSpriteMediaSetActionVariableSelect = 0x0111
kSpriteMediaGetActionVariableSelect = 0x0112
kSpriteMediaGetIndImagePropertySelect = 0x0113
kSpriteMediaNewSpriteSelect = 0x0114
kSpriteMediaDisposeSpriteSelect = 0x0115
kSpriteMediaSetActionVariableToStringSelect = 0x0116
kSpriteMediaGetActionVariableAsStringSelect = 0x0117
kFlashMediaSetPanSelect = 0x0101
kFlashMediaSetZoomSelect = 0x0102
kFlashMediaSetZoomRectSelect = 0x0103
kFlashMediaGetRefConBoundsSelect = 0x0104
kFlashMediaGetRefConIDSelect = 0x0105
kFlashMediaIDToRefConSelect = 0x0106
kFlashMediaGetDisplayedFrameNumberSelect = 0x0107
kFlashMediaFrameNumberToMovieTimeSelect = 0x0108
kFlashMediaFrameLabelToMovieTimeSelect = 0x0109
kMovieMediaGetChildDoMCActionCallbackSelect = 0x0102
kMovieMediaGetDoMCActionCallbackSelect = 0x0103
kMovieMediaGetCurrentMoviePropertySelect = 0x0104
kMovieMediaGetCurrentTrackPropertySelect = 0x0105
kMovieMediaGetChildMovieDataReferenceSelect = 0x0106
kMovieMediaSetChildMovieDataReferenceSelect = 0x0107
kMovieMediaLoadChildMovieFromDataReferenceSelect = 0x0108
kMedia3DGetNamedObjectListSelect = 0x0101
kMedia3DGetRendererListSelect = 0x0102
kMedia3DGetCurrentGroupSelect = 0x0103
kMedia3DTranslateNamedObjectToSelect = 0x0104
kMedia3DScaleNamedObjectToSelect = 0x0105
kMedia3DRotateNamedObjectToSelect = 0x0106
kMedia3DSetCameraDataSelect = 0x0107
kMedia3DGetCameraDataSelect = 0x0108
kMedia3DSetCameraAngleAspectSelect = 0x0109
kMedia3DGetCameraAngleAspectSelect = 0x010A
kMedia3DSetCameraRangeSelect = 0x010D
kMedia3DGetCameraRangeSelect = 0x010E
kMedia3DGetViewObjectSelect = 0x010F
kMCSetMovieSelect = 0x0002
kMCGetIndMovieSelect = 0x0005
kMCRemoveAllMoviesSelect = 0x0006
kMCRemoveAMovieSelect = 0x0003
kMCRemoveMovieSelect = 0x0006
kMCIsPlayerEventSelect = 0x0007
kMCSetActionFilterSelect = 0x0008
kMCDoActionSelect = 0x0009
kMCSetControllerAttachedSelect = 0x000A
kMCIsControllerAttachedSelect = 0x000B
kMCSetControllerPortSelect = 0x000C
kMCGetControllerPortSelect = 0x000D
kMCSetVisibleSelect = 0x000E
kMCGetVisibleSelect = 0x000F
kMCGetControllerBoundsRectSelect = 0x0010
kMCSetControllerBoundsRectSelect = 0x0011
kMCGetControllerBoundsRgnSelect = 0x0012
kMCGetWindowRgnSelect = 0x0013
kMCMovieChangedSelect = 0x0014
kMCSetDurationSelect = 0x0015
kMCGetCurrentTimeSelect = 0x0016
kMCNewAttachedControllerSelect = 0x0017
kMCDrawSelect = 0x0018
kMCActivateSelect = 0x0019
kMCIdleSelect = 0x001A
kMCKeySelect = 0x001B
kMCClickSelect = 0x001C
kMCEnableEditingSelect = 0x001D
kMCIsEditingEnabledSelect = 0x001E
kMCCopySelect = 0x001F
kMCCutSelect = 0x0020
kMCPasteSelect = 0x0021
kMCClearSelect = 0x0022
kMCUndoSelect = 0x0023
kMCPositionControllerSelect = 0x0024
kMCGetControllerInfoSelect = 0x0025
kMCSetClipSelect = 0x0028
kMCGetClipSelect = 0x0029
kMCDrawBadgeSelect = 0x002A
kMCSetUpEditMenuSelect = 0x002B
kMCGetMenuStringSelect = 0x002C
kMCSetActionFilterWithRefConSelect = 0x002D
kMCPtInControllerSelect = 0x002E
kMCInvalidateSelect = 0x002F
kMCAdjustCursorSelect = 0x0030
kMCGetInterfaceElementSelect = 0x0031
kMCGetDoActionsProcSelect = 0x0032
kMusicMediaGetIndexedTunePlayerSelect = 0x0101
