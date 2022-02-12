import * as React from 'react'

import MainPanel from './components/main-panel'
import ResourceList from './components/resource-list'
import { ViewType } from './state/component_types'
import DataContext from './state/context'

function Container () {
  const { viewType } = React.useContext(DataContext)

  if (viewType === ViewType.ResourceList) {
    return (<ResourceList />)
  }

  return (
    <MainPanel />
  )
}

export default Container
