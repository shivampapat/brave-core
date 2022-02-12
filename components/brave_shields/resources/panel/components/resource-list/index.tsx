import * as React from 'react'

import * as S from './style'
import DataContext from '../../state/context'
import { getLocale } from '../../../../../common/locale'
import TreeNode from './tree-node'
import { ViewType } from '../../state/component_types'

function ResourceList () {
  const { siteBlockInfo, setViewType } = React.useContext(DataContext)

  return (
    <S.Box>
      <S.HeaderBox>
        <S.SiteTitleBox>
          <S.SiteFavIcon />
          <S.SiteTitle>{siteBlockInfo?.host}</S.SiteTitle>
        </S.SiteTitleBox>
        <S.Grid>
          <span>{siteBlockInfo?.jsList.length}</span>
          <span>Blocked scripts</span>
          <S.ControlBox>
            {/* <S.AllowButton
              type="button"
            >
              Allow all
            </S.AllowButton> */}
          </S.ControlBox>
        </S.Grid>
      </S.HeaderBox>
      <S.Content>
        {siteBlockInfo?.jsList.map((entry, idx) => {
          return (<TreeNode
              key={idx}
              host={entry.host}
              resourceList={entry.resourceList}
            />)
        })}
      </S.Content>
      <S.Footer>
        <S.BackButton
          type="button"
          aria-label="Back to previous screen"
          onClick={() => setViewType?.(ViewType.Main)}
        >
          <svg fill="currentColor" viewBox="0 0 32 32" aria-hidden="true"><path d="M28 15H6.28l4.85-5.25a1 1 0 0 0-.05-1.42 1 1 0 0 0-1.41.06l-6.4 6.93a.7.7 0 0 0-.1.16.75.75 0 0 0-.09.15 1 1 0 0 0 0 .74.75.75 0 0 0 .09.15.7.7 0 0 0 .1.16l6.4 6.93a1 1 0 0 0 1.41.06 1 1 0 0 0 .05-1.42L6.28 17H28a1 1 0 0 0 0-2z"/></svg>
          <span>{getLocale('braveShields').replace('Brave ', '').trim()}</span>
        </S.BackButton>
      </S.Footer>
    </S.Box>
  )
}

export default ResourceList
