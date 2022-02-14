import * as React from 'react'
import { Checkbox } from 'brave-ui'
import { BraveWallet } from '../../../constants/types'
import { withPlaceholderIcon } from '../../shared'

// Utils
import Amount from '../../../utils/amount'

// Styled Components
import {
  StyledWrapper,
  AssetName,
  NameAndIcon,
  AssetIcon,
  DeleteButton,
  DeleteIcon,
  RightSide,
  NameAndSymbol,
  AssetSymbol
} from './style'

export interface Props {
  onSelectAsset: (key: string, selected: boolean, token: BraveWallet.BlockchainToken, isCustom: boolean) => void
  onRemoveAsset: (token: BraveWallet.BlockchainToken) => void
  isCustom: boolean
  isSelected: boolean
  token: BraveWallet.BlockchainToken
}

const AssetWatchlistItem = (props: Props) => {
  const {
    onSelectAsset,
    onRemoveAsset,
    isCustom,
    token,
    isSelected
  } = props

  const onCheck = (key: string, selected: boolean) => {
    onSelectAsset(key, selected, token, isCustom)
  }

  const onClickAsset = () => {
    onSelectAsset(token.contractAddress, !isSelected, token, isCustom)
  }

  const onClickRemoveAsset = () => {
    onRemoveAsset(token)
  }

  const AssetIconWithPlaceholder = React.useMemo(() => {
    return withPlaceholderIcon(AssetIcon, { size: 'big', marginLeft: 0, marginRight: 8 })
  }, [])

  return (
    <StyledWrapper>
      <NameAndIcon onClick={onClickAsset}>
        <AssetIconWithPlaceholder selectedAsset={token} />
        <NameAndSymbol>
          <AssetName>
            {token.name} {
              token.isErc721 && token.tokenId
                ? '#' + new Amount(token.tokenId).toNumber()
                : ''
            }
          </AssetName>
          <AssetSymbol>{token.symbol}</AssetSymbol>
        </NameAndSymbol>
      </NameAndIcon>
      <RightSide>
        {isCustom &&
          <DeleteButton onClick={onClickRemoveAsset}>
            <DeleteIcon />
          </DeleteButton>
        }
        <Checkbox value={{ [token.contractAddress]: isSelected }} onChange={onCheck}>
          <div data-key={token.contractAddress} />
        </Checkbox>
      </RightSide>
    </StyledWrapper>
  )
}

export default AssetWatchlistItem
