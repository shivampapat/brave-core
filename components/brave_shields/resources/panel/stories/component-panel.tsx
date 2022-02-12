import * as React from 'react'
import * as S from './style'
import { withKnobs } from '@storybook/addon-knobs'

import './locale'
import MainPanel from '../components/main-panel'
import ResourceList from '../components/resource-list'
import shieldsDarkTheme from '../theme/shields-dark'
import shieldsLightTheme from '../theme/shields-light'
import ThemeProvider from '../../../../common/StorybookThemeProvider'
import DataContext from '../state/context'
import { AdBlockMode, FingerprintMode, CookieBlockMode } from '../api/panel_browser_api'
import { ViewType } from '../state/component_types'

const LIST_JS = [
  {
    host: 'redditstatic.com',
    isTemporarilyAllowed: false,
    resourceList: [
      '/desktop2x/runtime~Reddit.5f2c92de54c3fab760f4.js',
      '/desktop2x/vendors~Governance~ModListing~Reddit~Subreddit.2dd75a68169a5f610955.js',
      '/desktop2x/vendors~Chat~Governance~Reddit.36bae844e1121d4994e8.js',
      '/desktop2x/vendors~PostCreation~Reddit~Subreddit.eb4233168e7670b3a272.js',
      '/desktop2x/PostCreation~Reddit~StandalonePostPage~SubredditTopContent~TopWeekPostsDiscoveryUnit~reddit-componen~2583c786.eea5d34d7a88aa637af3.js',
      '/desktop2x/PostCreation~Reddit~StandalonePostPage~reddit-components-ClassicPost~reddit-components-CompactPost~r~4c415e24.5ec9b9a42e98b7827b40.js',
      '/desktop2x/Governance~Reddit~Subreddit~reddit-components-BlankPost~reddit-components-ClassicPost~reddit-compone~3b56c92e.7244fc3d0e5919ee884c.js',
      '/desktop2x/PostCreation~Reddit~StandalonePostPage~Subreddit~reddit-components-ClassicPost~reddit-components-Com~82e48dd3.1244e7883a1b5116ee55.js',
      '/desktop2x/Reddit~StandalonePostPage~reddit-components-ClassicPost~reddit-components-CompactPost~reddit-compone~9b425435.cf57dddd51d15047d59b.js',
      '/desktop2x/Governance~ModListing~Reddit~ReportFlow~Subreddit.978c2cc5184cc31d7294.js',
      '/desktop2x/ModListing~PostCreation~Reddit~StandalonePostPage~Subreddit.4ef50437ae04288c0cc5.js',
      '/desktop2x/Reddit~reddit-components-ClassicPost~reddit-components-CompactPost~reddit-components-LargePost~reddi~90fdacc3.e2e4087dba804d4284e6.js',
      '/desktop2x/Governance~ModListing~Reddit~Subreddit.7d0128b5c86f6ec6e775.js',
      '/desktop2x/Chat~Governance~Reddit.7d719caf51b6acac1623.js',
      '/desktop2x/Governance~Reddit.3770c55b9d2dcf43f9c7.js',
      '/desktop2x/Reddit~Subreddit.7a7944ee2d660d3fb0b8.js',
      '/desktop2x/Reddit.6f239dde418379a3b203.js',
      '/desktop2x/js/xads.js',
      '/desktop2x/Frontpage.40539e7c212952846dd8.js',
      '/desktop2x/FeaturedLiveEntrypointAnnouncementsCarousel.ece6c179d3023e121ea5.js',
      '/desktop2x/reddit-components-InFeedPostCreation.930744fcd1f1c2b0e0f8.js',
      '/desktop2x/CollectionCommentsPage~CommentsPage~ModerationPages~PostCreation~ProfileComments~ProfileOverview~Pro~d39c0d57.00952b653cd1dcb5758a.js',
      '/desktop2x/CollectionCommentsPage~CommentsPage~ModerationPages~ProfileComments~ProfileOverview~ProfilePrivate~R~969c2956.ae6a91c29c6642bb46d4.js',
      '/desktop2x/reddit-components-LargePost.62db2bccf3855f2b3cb9.js',
      '/desktop2x/IdCard.fc724',
      '/desktop2x/CollectionCommentsPage~CommentsPage~ModerationPages~PostCreation~ProfileComments~ProfileOverview~Pro~d39c0d57.00952b653cd1dcb5758a.js'
    ]
  },
  {
    host: 'https://figma.com',
    isTemporarilyAllowed: false,
    resourceList: [
      'static.figma.com',
      'interactive.figma.com',
      'reddit.assets.images.figma.com',
      'reddit.assets.images.figma.com',
      'interactive.figma.com',
      'https://www.redditstatic.com/desktop2x/vendors~Governance~ModListing~Reddit~Subreddit.2dd75a68169a5f610955.js',
      'interactive.figma.com',
      'reddit.assets.images.figma.com',
      'reddit.assets.images.figma.com',
      'interactive.figma.com'
    ]
  },
  {
    host: 'https://api.segment.io/v1/identify',
    isTemporarilyAllowed: false,
    resourceList: []
  },
  {
    host: 'https://s.amazon-adsystem.com/iu3?d=amazon.com&slot=navFooter&a1=010181903ad2e0f5681400053e118e937f53fdddeb45615aedef14cf28e9b3fbeddf&a2=01015a0a66fe5db9090fff2a4b3f63c94aa1654f78683ec95e793269d1ac5846c339&old_oo=0&ts=1644273837651&s=AQ7a7PLeChFiDK93z0GO5VI2NFA7onHyIZxj-mtw3C_U&gdpr_consent=&gdpr_consent_avl=&cb=1644273837651',
    isTemporarilyAllowed: false,
    resourceList: []
  },
  {
    host: 'https://buzzfeed.com',
    isTemporarilyAllowed: false,
    resourceList: []
  }
]

const LIST_ADS = [
  { url: 'ads.brave.com' },
  { url: 'ads2.brave.com' }
]

export default {
  title: 'ShieldsV2/Panels',
  parameters: {
    layout: 'centered'
  },
  argTypes: {
    locked: { control: { type: 'boolean', lock: false } }
  },
  decorators: [
    (Story: any) => {
      // mock data
      const store = {
        siteBlockInfo: {
          host: 'brave.com',
          totalBlockedResources: 2,
          isShieldsEnabled: true,
          adsList: LIST_ADS,
          jsList: LIST_JS,
          httpRedirectsList: [],
          fingerprintsList: []
        },
        siteSettings: {
          adBlockMode: AdBlockMode.ALLOW,
          fingerprintMode: FingerprintMode.ALLOW,
          cookieBlockMode: CookieBlockMode.ALLOW,
          isHttpsEverywhereEnabled: true,
          isNoscriptEnabled: false
        },
        viewType: ViewType.Main
      }

      return (
        <DataContext.Provider value={store}>
          <ThemeProvider
            darkTheme={shieldsDarkTheme}
            lightTheme={shieldsLightTheme}
          >
            <Story />
          </ThemeProvider>
        </DataContext.Provider>
      )
    },
    withKnobs
  ]
}

export const _Main = () => {
  return (
    <S.PanelFrame>
      <MainPanel />
    </S.PanelFrame>
  )
}

export const _ResourceList = () => {
  return (
    <S.PanelFrame>
      <ResourceList />
    </S.PanelFrame>
  )
}
