import * as React from 'react'

import { BraveWallet } from '../../../constants/types'
import { withPlaceholderIcon } from '../../shared'

// Utils
import Amount from '../../../utils/amount'

// Styled Components
import {
  StyledWrapper,
  AssetBalance,
  AssetAndBalance,
  AssetName,
  AssetIcon
} from './style'

export interface Props {
  asset: BraveWallet.BlockchainToken
  onSelectAsset: () => void
}

function SelectAssetItem (props: Props) {
  const { asset, onSelectAsset } = props

  const AssetIconWithPlaceholder = React.useMemo(() => {
    return withPlaceholderIcon(AssetIcon, { size: 'small', marginLeft: 0, marginRight: 8 })
  }, [])

  return (
    <StyledWrapper onClick={onSelectAsset}>
      <AssetIconWithPlaceholder selectedAsset={asset} />
      <AssetAndBalance>
        <AssetName>
          {asset.name} {
            asset.isErc721 && asset.tokenId
              ? '#' + new Amount(asset.tokenId).toNumber()
              : ''
          }
        </AssetName>
        <AssetBalance>{asset.symbol}</AssetBalance>
      </AssetAndBalance>
    </StyledWrapper>
  )
}

export default SelectAssetItem
