import styled from 'styled-components'

export const Box = styled.div`
  background-color: ${(p) => p.theme.color.background02};
  width: 100%;
  height: 100%;
`

export const HeaderBox = styled.div`
  width: 100%;
  padding: 24px 24px 16px 24px;
`

export const SiteTitleBox = styled.div`
  display: flex;
  align-items: center;
  margin-bottom: 15px;
`

export const SiteTitle = styled.span`
  color: ${(p) => p.theme.color.text01};
  font-family: ${(p) => p.theme.fontFamily.heading};
  font-size: 14px;
  font-weight: 500;
`

export const SiteFavIcon = styled.span`
  width: 18px;
  height: 18px;
  background-color: #ededed;
  display: inline-block;
  margin-right: 8px;
`

export const BackButton = styled.button`
  display: flex;
  align-items: center;
  justify-content: center;
  padding: 10px 22px;
  width: 100%;

  background-color: transparent;
  border: 1px solid ${(p) => p.theme.color.interactive08};
  border-radius: 48px;

  color: ${(p) => p.theme.color.interactive05};
  font-family: ${(p) => p.theme.fontFamily.heading};
  font-size: 13px;
  font-weight: 600;

  svg {
    width: 22px;
    height: 22px;
    margin-right: 8px;
  }

  svg > path {
    color: currentColor;
  }
`

export const Grid = styled.div`
  display: grid;
  grid-template-columns: 20px 2fr 1fr;
  grid-gap: 5px;
  align-items: center;
  font-family: ${(p) => p.theme.fontFamily.heading};
  color: ${(p) => p.theme.color.text01};
  font-size: 14px;
  font-weight: 600;
`

export const Content = styled.div`
  background-color: ${(p) => p.theme.color.background01};
  padding: 10px 24px 10px 24px;
  max-height: 300px;
  overflow-y: scroll;
  position: relative;
`

export const ControlBox = styled.span`
  margin-left: auto;
`

export const Footer = styled.div`
  padding: 24px;
`

export const AllowButton = styled.button`
  background-color: transparent;
  border: 0;
  padding: 0;
  margin: 0;
  color: ${(p) => p.theme.color.interactive05};
  font-family: ${(p) => p.theme.fontFamily.heading};
  font-weight: normal;
  font-size: 12px;
  text-decoration-line: underline;
`
